#ifndef BANDWIDTHCROSSLAYER_ALGORITHM_H
#define BANDWIDTHCROSSLAYER_ALGORITHM_H
#include "tcp-stream-bandwidth-algorithm.h"

namespace ns3 {

class BandwidthCrosslayerAlgorithm : public BandwidthAlgorithm {
 public:
  BandwidthCrosslayerAlgorithm(const videoData &videoData,
                               const playbackData &playbackData,
                               const bufferData &bufferData,
                               const throughputData &throughput);

  bandwidthAlgoReply BandwidthAlgo(
      const int64_t segmentCounter, const int64_t clientId,
      int64_t extraParameter,    // crosslayerBandwidth
      int64_t extraParameter2);  // reservation
 private:
  const int64_t m_bandwidthAlgoIndex;
  double m_lastBandwidthEstimate;  // Last bandwidthEstimate Value
  const int64_t m_highestRepIndex;
};

}  // namespace ns3
#endif /* BANDWIDTHCROSSLAYER_ALGORITHM_H */