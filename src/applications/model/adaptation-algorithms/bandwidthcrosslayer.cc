#include "bandwidthcrosslayer.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("BandwidthCrosslayerAlgorithm");
NS_OBJECT_ENSURE_REGISTERED(BandwidthCrosslayerAlgorithm);

BandwidthCrosslayerAlgorithm::BandwidthCrosslayerAlgorithm(
    const videoData &videoData, const playbackData &playbackData,
    const bufferData &bufferData, const throughputData &throughput)
    : BandwidthAlgorithm(videoData, playbackData, bufferData, throughput),
      m_bandwidthAlgoIndex(1),
      m_lastBandwidthEstimate(0),  // default bandwidthEstimate value = 0
      m_highestRepIndex(videoData.averageBitrate[0].size() - 1) {
  NS_LOG_INFO(this);
  NS_ASSERT_MSG(m_highestRepIndex >= 0,
                "The highest quality representation index should be >= 0");
}

bandwidthAlgoReply BandwidthCrosslayerAlgorithm::BandwidthAlgo(
    const int64_t segmentCounter, const int64_t clientId,
    int64_t extraParameter,   // crosslayerBandwdith
    int64_t extraParameter2)  // reservation
{
  bandwidthAlgoReply answer;
  answer.bandwidthAlgoIndex = 2;
  const int64_t timeNow = Simulator::Now().GetMicroSeconds();
  answer.decisionTime = timeNow;

  if (segmentCounter != 0) {
    if (extraParameter > 0) {
      m_lastBandwidthEstimate = extraParameter;
      answer.decisionCase = 2;
    } else {
      m_lastBandwidthEstimate = m_lastBandwidthEstimate;
      answer.decisionCase = 1;
    }
  } else {
    m_lastBandwidthEstimate = m_lastBandwidthEstimate;
    answer.decisionCase = 0;
  }
  answer.bandwidthEstimate = m_lastBandwidthEstimate;
  return answer;
}

}  // namespace ns3