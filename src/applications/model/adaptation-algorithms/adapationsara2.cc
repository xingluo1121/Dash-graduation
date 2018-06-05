#include "adapationsara2.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("Sara2Algorithm");
NS_OBJECT_ENSURE_REGISTERED(Sara2Algorithm);

Sara2Algorithm::Sara2Algorithm(const videoData &videoData,
                               const playbackData &playbackData,
                               const bufferData &bufferData,
                               const throughputData &throughput)
    : AdaptationAlgorithm(videoData, playbackData, bufferData, throughput),
      m_lastRepIndex(0),
      m_bufferHigh(m_videoData.segmentDuration * 6),
      m_bufferLow(m_videoData.segmentDuration * 4),
      m_bufferMin(m_videoData.segmentDuration * 2),
      m_alpha1(0.40),
      m_alpha2(0.80),
      m_beta(0.10),
      m_deltaTime(m_videoData.segmentDuration),
      m_gama1(0.95),
      m_gama2(1.05),
      m_gama3(1.00),
      m_highestRepIndex(videoData.averageBitrate[0].size() - 1) {
  NS_LOG_INFO(this);
  NS_ASSERT_MSG(m_highestRepIndex >= 0,
                "The highest quality representation index should be >= 0");
}

algorithmReply Sara2Algorithm::GetNextRep(const int64_t segmentCounter,
                                          const int64_t clientId,
                                          int64_t extraParameter,
                                          int64_t extraParameter2) {
  algorithmReply answer;
  answer.decisionCase = 0;
  answer.delayDecisionCase = 0;
  answer.nextDownloadDelay = 0;
  const int64_t timeNow = Simulator::Now().GetMicroSeconds();
  answer.decisionTime = timeNow;
  int64_t bufferNow = 0;

  if (segmentCounter == 0) {
    Normalize(m_videoData, m_normalized,
              m_normalizationConstant);  // repsLevel   reps at(0)=1
  }
  if (segmentCounter != 0) {
    int64_t n_segmentSizePrediction =
        Esmoothing(m_videoData, segmentCounter, m_repIndex, m_normalized,
                   m_normalizationConstant, m_prediction1, m_prediction2,
                   m_prediction3, m_prediction4);  // bit
    bufferNow = m_bufferData.bufferLevelNew.back() -
                (timeNow - m_throughput.transmissionEnd.back());
    if (bufferNow <= m_bufferMin) {
      answer.nextRepIndex = 0;
      answer.decisionCase = 1;
    } else {
      m_gama3 = (bufferNow - m_bufferMin) / 1000000.0 / 8 / 10 + 1.02;
      if (m_normalized.at(m_lastRepIndex) * n_segmentSizePrediction /
                  extraParameter >
              (bufferNow - m_bufferMin) / 1000000.0 ||
          m_normalized.at(m_lastRepIndex) * n_segmentSizePrediction /
                  extraParameter >
              m_deltaTime * m_gama2 ||
          m_videoData.averageBitrate.at(m_videoData.userInfo.at(segmentCounter))
                      .at(m_lastRepIndex) /
                  extraParameter >
              m_gama3) {
        int nextRepIndex;
        for (nextRepIndex = 0; nextRepIndex < m_lastRepIndex; nextRepIndex++) {
          if ((m_normalized.at(nextRepIndex) * n_segmentSizePrediction) /
                  extraParameter >
              (bufferNow - m_bufferMin) / 1000000.0)
            break;
        }
        answer.nextRepIndex = nextRepIndex > 0 ? nextRepIndex - 1 : 0;
        answer.decisionCase = 3;
      } else if (bufferNow <= m_bufferLow) {
        if (m_lastRepIndex < m_highestRepIndex &&
            m_normalized.at(m_lastRepIndex + 1) * n_segmentSizePrediction /
                    extraParameter <=
                (bufferNow - m_bufferMin) / 1000000.0 &&
            m_normalized.at(m_lastRepIndex + 1) * n_segmentSizePrediction /
                    extraParameter <=
                m_deltaTime * m_gama1 &&
            m_videoData.averageBitrate
                        .at(m_videoData.userInfo.at(segmentCounter))
                        .at(m_lastRepIndex + 1) /
                    extraParameter <=
                m_gama3) {
          answer.nextRepIndex = m_lastRepIndex + 1;
          answer.decisionCase = 4;
        } else {
          answer.nextRepIndex = m_lastRepIndex;
          answer.decisionCase = 5;
        }
      } else if (bufferNow <= m_bufferHigh) {
        int nextRepIndex;
        for (nextRepIndex = m_highestRepIndex; nextRepIndex >= m_lastRepIndex;
             nextRepIndex--) {
          if (m_normalized.at(nextRepIndex) * n_segmentSizePrediction /
                      extraParameter <=
                  (bufferNow - m_bufferMin * 2) / 1000000.0 &&
              m_normalized.at(m_lastRepIndex) * n_segmentSizePrediction /
                      extraParameter <=
                  m_deltaTime * m_gama2 &&
              m_videoData.averageBitrate
                          .at(m_videoData.userInfo.at(segmentCounter))
                          .at(m_lastRepIndex) /
                      extraParameter <=
                  m_gama3)
            break;
        }
        answer.nextRepIndex = nextRepIndex;
        answer.decisionCase = 6;
      } else if (bufferNow > m_bufferHigh) {
        int nextRepIndex;
        for (nextRepIndex = m_highestRepIndex; nextRepIndex >= m_lastRepIndex;
             nextRepIndex--) {
          if (m_normalized.at(nextRepIndex) * n_segmentSizePrediction /
                      extraParameter <=
                  (bufferNow - m_bufferLow) / 1000000.0 &&
              (m_normalized.at(m_lastRepIndex) * n_segmentSizePrediction) /
                      extraParameter <=
                  m_deltaTime * m_gama2 &&
              m_videoData.averageBitrate
                          .at(m_videoData.userInfo.at(segmentCounter))
                          .at(m_lastRepIndex) /
                      extraParameter <=
                  m_gama3)
            break;
        }
        answer.nextRepIndex = nextRepIndex;
        answer.decisionCase = 7;
      } else {
        answer.nextRepIndex = m_lastRepIndex;
        answer.decisionCase = 2;
      }
    }

    if (bufferNow > m_bufferHigh) {
      answer.nextDownloadDelay = (int64_t)(bufferNow - m_bufferHigh);
      answer.delayDecisionCase = 1;
    }
  } else {
    answer.nextRepIndex = m_lastRepIndex;
    answer.decisionCase = 0;
  }
  answer.estimateTh = extraParameter;
  m_lastRepIndex = answer.nextRepIndex;
  m_repIndex.push_back(m_lastRepIndex);
  return answer;
}

void Sara2Algorithm::Normalize(const videoData &videoData,
                               std::vector<double> &normalized,
                               double &normalizationConstant) {
  int userInfoSize = videoData.averageBitrate.size();
  int repLevelSize = videoData.averageBitrate[0].size();
  double normalization = 0;
  for (int i = 0; i < repLevelSize; i++) {
    for (int j = 0; j < userInfoSize; j++) {
      if (j == 0) {
        normalized.push_back(
            videoData.averageBitrate.at(videoData.userInfo.at(j)).at(i));
      } else {
        normalized.at(i) +=
            videoData.averageBitrate.at(videoData.userInfo.at(j)).at(i);
      }
    }
    normalized.at(i) = normalized.at(i) / (double)userInfoSize;
  }
  normalization = normalized.at(0);
  normalizationConstant =
      normalization * (videoData.segmentDuration /
                       1000000.0);  // Normalized Average Segment Size
  for (int i = 0; i < repLevelSize; i++) {
    normalized.at(i) = normalized.at(i) / normalization;
  }
}
int64_t Sara2Algorithm::Esmoothing(
    const videoData &videoData, const int64_t &segmentCounter,
    const std::vector<int64_t> &repIndex, const std::vector<double> &normalized,
    const double &normalization, std::vector<double> &prediction1,
    std::vector<double> &prediction2, std::vector<double> &prediction3,
    std::vector<double> &prediction4) {
  double predictionValue = 0;
  if (segmentCounter == 1) {
    double n_lastSegmentSize =
        videoData.segmentSize.at(videoData.userInfo.at(segmentCounter - 1))
            .at(repIndex.at(segmentCounter - 1))
            .at(segmentCounter - 1) *
        8.0 / normalized.at(repIndex.at(segmentCounter - 1));
    prediction1.push_back(n_lastSegmentSize);
    prediction2.push_back(n_lastSegmentSize);
    prediction3.push_back(n_lastSegmentSize);
    prediction4.push_back(n_lastSegmentSize);
    predictionValue = n_lastSegmentSize;
  } else {
    double n_lastSegmentSize =
        videoData.segmentSize.at(videoData.userInfo.at(segmentCounter - 1))
            .at(repIndex.at(segmentCounter - 1))
            .at(segmentCounter - 1) *
        8.0 / normalized.at(repIndex.at(segmentCounter - 1));
    double deltaInP2 = abs(normalization - prediction2.back());
    prediction1.push_back(prediction1.back() * (1 - m_alpha1) +
                          n_lastSegmentSize * m_alpha1);
    prediction2.push_back(prediction2.back() * (1 - m_alpha2) +
                          prediction1.back() * m_alpha2);
    prediction3.push_back(n_lastSegmentSize);
    double a = 2 * prediction1.at(segmentCounter - 1) -
               prediction2.at(segmentCounter - 1);
    double b =
        (m_alpha2 / (1 - m_alpha2) * (prediction1.back() - prediction2.back()));
    double ES2 = a + b * 1;
    double beta = m_beta * deltaInP2 / normalization;
    prediction4.push_back(ES2 * (1 - beta) + normalization * beta);
    predictionValue = prediction4.back();
  }
  return predictionValue;
}

}  // namespace ns3