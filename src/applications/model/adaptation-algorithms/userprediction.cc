#include "userprediction.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("UserPredictionAlgorithm");
NS_OBJECT_ENSURE_REGISTERED(UserPredictionAlgorithm);

UserPredictionAlgorithm::UserPredictionAlgorithm(
    const videoData &videoData, const playbackData &playbackData,
    const bufferData &bufferData, const throughputData &throughput)
    : UserinfoAlgorithm(videoData, playbackData, bufferData, throughput) {
  NS_LOG_INFO(this);
}

userinfoAlgoReply UserPredictionAlgorithm::UserinfoAlgo(
    const int64_t segmentCounter, const int64_t clientId,
    int64_t extraParameter, int64_t extraParameter2) {
  userinfoAlgoReply answer;
  const int64_t timeNow = Simulator::Now().GetMicroSeconds();
  answer.decisionTime = timeNow;

  if (extraParameter == 0) {
    answer.bufferCleanNumber = 0;    // no buffer clean
    answer.bufferTargetNumber = 10;  // constant
  } else {
    // if() p(trans)->buffer clean number, relationship ?
    answer.bufferCleanNumber = 0;  // to be specific
    // if() p(trans)->bufferTargetnumber, relationship?
    answer.bufferTargetNumber = 10;
  }
  return answer;
}

}  // namespace ns3