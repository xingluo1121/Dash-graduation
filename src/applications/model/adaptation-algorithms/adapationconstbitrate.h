#ifndef CONSTBITRATE_ALGORITHM_H
#define CONSTBITRATE_ALGORITHM_H

#include "tcp-stream-adaptation-algorithm.h"

namespace ns3 {

class constbitrateAlgorithm : public AdaptationAlgorithm {
 public:
  constbitrateAlgorithm(const videoData &videoData,
                        const playbackData &playbackData,
                        const bufferData &bufferData,
                        const throughputData &throughput);

  algorithmReply GetNextRep(const int64_t segmentCounter,
                            const int64_t clientId, int64_t extraParameter,
                            int64_t extraParameter2);

 private:
  const int64_t m_targetBuffer;
  const int64_t m_deltaBuffer;
  const int64_t m_highestRepIndex;
  const int64_t m_constRepIndex;
};

}  // namespace ns3
#endif /* CONSTBITRATE_ALGORITHM_H */