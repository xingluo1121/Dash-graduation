#ifndef TOMATO_ALGORITHM_H
#define TOMATO_ALGORITHM_H

#include "tcp-stream-adaptation-algorithm.h"

namespace ns3 {

class TomatoAlgorithm : public AdaptationAlgorithm {
 public:
  TomatoAlgorithm(const videoData &videoData, const playbackData &playbackData,
                  const bufferData &bufferData,
                  const throughputData &throughput);

  algorithmReply GetNextRep(const int64_t segmentCounter,
                            const int64_t clientId, int64_t extraParameter,
                            int64_t extraParameter2);

 private:
  int64_t m_lastRepIndex;
  int64_t m_targetBuffer;
  int64_t m_deltaBuffer;
  int64_t m_bufferMin;
  const int64_t m_highestRepIndex;
};

}  // namespace ns3
#endif /* TOMATO_ALGORITHM_H */
