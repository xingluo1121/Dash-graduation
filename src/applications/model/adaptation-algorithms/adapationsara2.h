#ifndef SARA2_ALGORITHM_H
#define SARA2_ALGORITHM_H

#include "tcp-stream-adaptation-algorithm.h"

namespace ns3 {
// do not use this algo
class Sara2Algorithm : public AdaptationAlgorithm {
 public:
  Sara2Algorithm(const videoData &videoData, const playbackData &playbackData,
                 const bufferData &bufferData,
                 const throughputData &throughput);

  algorithmReply GetNextRep(const int64_t segmentCounter,
                            const int64_t clientId, int64_t extraParameter,
                            int64_t extraParameter2);

 private:
  void Normalize(const videoData &videoData, std::vector<double> &normalized,
                 double &normalizationConstant);
  int64_t Esmoothing(const videoData &videoData, const int64_t &segmentCounter,
                     const std::vector<int64_t> &repIndex,
                     const std::vector<double> &normalized,
                     const double &normalization,
                     std::vector<double> &prediction1,
                     std::vector<double> &prediction2,
                     std::vector<double> &prediction3,
                     std::vector<double> &prediction4);

  int64_t m_lastRepIndex;
  int64_t m_bufferHigh;
  int64_t m_bufferLow;
  int64_t m_bufferMin;
  double m_deltaTimeH;
  double m_deltaTimeL;
  double m_alpha1;
  double m_alpha2;
  double m_beta;
  double m_deltaTime;
  double m_gama1;
  double m_gama2;
  double m_gama3;
  std::vector<double> m_normalized;
  std::vector<int64_t> m_repIndex;
  std::vector<double> m_prediction1;  // S1
  std::vector<double> m_prediction2;  // S2
  std::vector<double> m_prediction3;  // last -> alpha =1 ES1
  std::vector<double> m_prediction4;  // improvedS2 = S2*(1-a) + a * average a =
                                      // alpha3 * abs( delta S2' / average )
  double m_normalizationConstant;
  const int64_t m_highestRepIndex;
};

}  // namespace ns3
#endif /* SARA2_ALGORITHM_H */
