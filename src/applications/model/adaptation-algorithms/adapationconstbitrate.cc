#include "adapationconstbitrate.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("constbitrateAlgorithm");
NS_OBJECT_ENSURE_REGISTERED(constbitrateAlgorithm);

constbitrateAlgorithm::constbitrateAlgorithm(const videoData &videoData,
                                             const playbackData &playbackData,
                                             const bufferData &bufferData,
                                             const throughputData &throughput)
    : AdaptationAlgorithm(videoData, playbackData, bufferData, throughput),
      m_targetBuffer(m_videoData.segmentDuration * 10),  // 10s
      m_deltaBuffer(m_videoData.segmentDuration * 2),    // 2s
      m_highestRepIndex(videoData.averageBitrate[0].size() - 1),
      m_constRepIndex(4) {
  NS_LOG_INFO(this);
  NS_ASSERT_MSG(m_highestRepIndex >= 0,
                "The highest quality representation index should be => 0");
}

algorithmReply constbitrateAlgorithm::GetNextRep(const int64_t segmentCounter,
                                                 int64_t clientId,
                                                 int64_t extraParameter,
                                                 int64_t extraParameter2) {
  algorithmReply answer;
  int64_t timeNow = Simulator::Now().GetMicroSeconds();
  answer.decisionTime = timeNow;
  answer.decisionCase = 0;
  answer.nextRepIndex = m_constRepIndex;
  answer.estimateTh = extraParameter;

  if (segmentCounter != 0) {
    int64_t lowerBound = m_targetBuffer - m_deltaBuffer;
    int64_t upperBound = m_targetBuffer + m_deltaBuffer;
    int64_t bufferNow = m_bufferData.bufferLevelNew.back() -
                        (timeNow - m_throughput.transmissionEnd.back());
    // buffer control
    int64_t randBuf =
        lowerBound + (int64_t)(std::rand() % (upperBound - (lowerBound) + 1));
    if (bufferNow > randBuf) {
      answer.nextDownloadDelay = bufferNow - randBuf;
      answer.nextDownloadDelay = 0;
      answer.delayDecisionCase = 1;
    } else {
      answer.nextDownloadDelay = 0;
      answer.delayDecisionCase = 0;
    }
  }
  return answer;
}

}  // namespace ns3
