#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "json.hpp"

#include "NAM/get_dsp.h"

namespace test_get_dsp
{
// Config
const std::string basicConfigStr =
  R"({"version": "0.5.4", "metadata": {"date": {"year": 2024, "month": 10, "day": 9, "hour": 18, "minute": 44, "second": 41}, "loudness": -37.8406867980957, "gain": 0.13508800804658277, "name": "Test LSTM", "modeled_by": "Steve", "gear_type": "amp", "gear_make": "Darkglass Electronics", "gear_model": "Microtubes 900 v2", "tone_type": "clean", "input_level_dbu": 18.3, "output_level_dbu": 12.3, "training": {"settings": {"ignore_checks": false}, "data": {"latency": {"manual": null, "calibration": {"algorithm_version": 1, "delays": [-16], "safety_factor": 1, "recommended": -17, "warnings": {"matches_lookahead": false, "disagreement_too_high": false}}}, "checks": {"version": 3, "passed": true}}, "validation_esr": null}}, "architecture": "LSTM", "config": {"input_size": 1, "hidden_size": 3, "num_layers": 1}, "weights": [-0.21677088737487793, -0.6683622002601624, -0.2560940980911255, -0.3588429093360901, 0.17952610552310944, 0.19445613026618958, -0.01662646047770977, 0.5353694558143616, -0.2536540627479553, -0.5132213234901428, -0.020476307719945908, 0.08592455089092255, -0.6891753673553467, 0.3627359867095947, 0.008421811275184155, 0.3113192617893219, 0.14251480996608734, 0.07989779114723206, -0.18211324512958527, 0.7118963003158569, 0.41084015369415283, -0.6571938395500183, -0.13214066624641418, -0.2698603868484497, 0.49387243390083313, -0.3491725027561188, 0.6353667974472046, -0.5005152225494385, 0.2052856683731079, -0.4301638901233673, -0.15770092606544495, -0.7181791067123413, 0.056290093809366226, -0.49049463868141174, 0.6623441576957703, 0.09029324352741241, 0.34005245566368103, 0.16416560113430023, 0.15520110726356506, -0.4155678153038025, -0.36928507685661316, 0.3211132884025574, -0.6769840121269226, -0.1575538069009781, 0.05268515646457672, -0.4191459119319916, 0.599330484867096, 0.21518059074878693, -4.246325492858887, -3.315647840499878, -4.328850746154785, 4.496089458465576, 5.015639305114746, 3.6492037773132324, 0.14431169629096985, -0.6633821725845337, 0.11673200130462646, -0.1418764889240265, -0.4897872805595398, -0.8689419031143188, -0.06714004278182983, -0.4450395107269287, -0.02142983116209507, -0.15136894583702087, -2.775207996368408, -0.08681213855743408, 0.05702732503414154, 0.670292317867279, 0.31442636251449585, 0.30793967843055725], "sample_rate": 48000})";

// Copied over but shouldn't be publicly-exposed.
std::vector<float> GetWeights(nlohmann::json const& j)
{
  auto it = j.find("weights");
  if (it != j.end())
  {
    return *it;
  }
  else
  {
    throw std::runtime_error("Corrupted model file is missing weights.");
  }
}

nam::dspData _GetConfig(const std::string& configStr = basicConfigStr)
{
  nlohmann::json j = nlohmann::json::parse(configStr);

  std::vector<float> weights = GetWeights(j);
  nam::dspData returnedConfig;
  returnedConfig.version = j["version"];
  returnedConfig.architecture = j["architecture"];
  returnedConfig.config = j["config"];
  returnedConfig.metadata = j["metadata"];
  returnedConfig.weights = weights;

  return returnedConfig;
}

void test_gets_input_level()
{
  nam::dspData config = _GetConfig();
  std::unique_ptr<nam::DSP> dsp = get_dsp(config);
  assert(dsp->HasInputLevel());
}
void test_gets_output_level()
{
  nam::dspData config = _GetConfig();
  std::unique_ptr<nam::DSP> dsp = get_dsp(config);
  assert(dsp->HasOutputLevel());
}

void test_null_input_level()
{
  // Issue 129
  const std::string configStr =
    R"({"version": "0.5.4", "metadata": {"date": {"year": 2024, "month": 10, "day": 9, "hour": 18, "minute": 44, "second": 41}, "loudness": -37.8406867980957, "gain": 0.13508800804658277, "name": "Test LSTM", "modeled_by": "Steve", "gear_type": "amp", "gear_make": "Darkglass Electronics", "gear_model": "Microtubes 900 v2", "tone_type": "clean", "input_level_dbu": null, "output_level_dbu": 12.3, "training": {"settings": {"ignore_checks": false}, "data": {"latency": {"manual": null, "calibration": {"algorithm_version": 1, "delays": [-16], "safety_factor": 1, "recommended": -17, "warnings": {"matches_lookahead": false, "disagreement_too_high": false}}}, "checks": {"version": 3, "passed": true}}, "validation_esr": null}}, "architecture": "LSTM", "config": {"input_size": 1, "hidden_size": 3, "num_layers": 1}, "weights": [-0.21677088737487793, -0.6683622002601624, -0.2560940980911255, -0.3588429093360901, 0.17952610552310944, 0.19445613026618958, -0.01662646047770977, 0.5353694558143616, -0.2536540627479553, -0.5132213234901428, -0.020476307719945908, 0.08592455089092255, -0.6891753673553467, 0.3627359867095947, 0.008421811275184155, 0.3113192617893219, 0.14251480996608734, 0.07989779114723206, -0.18211324512958527, 0.7118963003158569, 0.41084015369415283, -0.6571938395500183, -0.13214066624641418, -0.2698603868484497, 0.49387243390083313, -0.3491725027561188, 0.6353667974472046, -0.5005152225494385, 0.2052856683731079, -0.4301638901233673, -0.15770092606544495, -0.7181791067123413, 0.056290093809366226, -0.49049463868141174, 0.6623441576957703, 0.09029324352741241, 0.34005245566368103, 0.16416560113430023, 0.15520110726356506, -0.4155678153038025, -0.36928507685661316, 0.3211132884025574, -0.6769840121269226, -0.1575538069009781, 0.05268515646457672, -0.4191459119319916, 0.599330484867096, 0.21518059074878693, -4.246325492858887, -3.315647840499878, -4.328850746154785, 4.496089458465576, 5.015639305114746, 3.6492037773132324, 0.14431169629096985, -0.6633821725845337, 0.11673200130462646, -0.1418764889240265, -0.4897872805595398, -0.8689419031143188, -0.06714004278182983, -0.4450395107269287, -0.02142983116209507, -0.15136894583702087, -2.775207996368408, -0.08681213855743408, 0.05702732503414154, 0.670292317867279, 0.31442636251449585, 0.30793967843055725], "sample_rate": 48000})";
  nam::dspData config = _GetConfig(configStr);
  // The first part of this is that the following line doesn't fail:
  std::unique_ptr<nam::DSP> dsp = get_dsp(config);

  assert(!dsp->HasInputLevel());
  assert(dsp->HasOutputLevel());
}

void test_null_output_level()
{
  // Issue 129
  const std::string configStr =
    R"({"version": "0.5.4", "metadata": {"date": {"year": 2024, "month": 10, "day": 9, "hour": 18, "minute": 44, "second": 41}, "loudness": -37.8406867980957, "gain": 0.13508800804658277, "name": "Test LSTM", "modeled_by": "Steve", "gear_type": "amp", "gear_make": "Darkglass Electronics", "gear_model": "Microtubes 900 v2", "tone_type": "clean", "input_level_dbu": 19.0, "output_level_dbu": null, "training": {"settings": {"ignore_checks": false}, "data": {"latency": {"manual": null, "calibration": {"algorithm_version": 1, "delays": [-16], "safety_factor": 1, "recommended": -17, "warnings": {"matches_lookahead": false, "disagreement_too_high": false}}}, "checks": {"version": 3, "passed": true}}, "validation_esr": null}}, "architecture": "LSTM", "config": {"input_size": 1, "hidden_size": 3, "num_layers": 1}, "weights": [-0.21677088737487793, -0.6683622002601624, -0.2560940980911255, -0.3588429093360901, 0.17952610552310944, 0.19445613026618958, -0.01662646047770977, 0.5353694558143616, -0.2536540627479553, -0.5132213234901428, -0.020476307719945908, 0.08592455089092255, -0.6891753673553467, 0.3627359867095947, 0.008421811275184155, 0.3113192617893219, 0.14251480996608734, 0.07989779114723206, -0.18211324512958527, 0.7118963003158569, 0.41084015369415283, -0.6571938395500183, -0.13214066624641418, -0.2698603868484497, 0.49387243390083313, -0.3491725027561188, 0.6353667974472046, -0.5005152225494385, 0.2052856683731079, -0.4301638901233673, -0.15770092606544495, -0.7181791067123413, 0.056290093809366226, -0.49049463868141174, 0.6623441576957703, 0.09029324352741241, 0.34005245566368103, 0.16416560113430023, 0.15520110726356506, -0.4155678153038025, -0.36928507685661316, 0.3211132884025574, -0.6769840121269226, -0.1575538069009781, 0.05268515646457672, -0.4191459119319916, 0.599330484867096, 0.21518059074878693, -4.246325492858887, -3.315647840499878, -4.328850746154785, 4.496089458465576, 5.015639305114746, 3.6492037773132324, 0.14431169629096985, -0.6633821725845337, 0.11673200130462646, -0.1418764889240265, -0.4897872805595398, -0.8689419031143188, -0.06714004278182983, -0.4450395107269287, -0.02142983116209507, -0.15136894583702087, -2.775207996368408, -0.08681213855743408, 0.05702732503414154, 0.670292317867279, 0.31442636251449585, 0.30793967843055725], "sample_rate": 48000})";
  nam::dspData config = _GetConfig(configStr);
  // The first part of this is that the following line doesn't fail:
  std::unique_ptr<nam::DSP> dsp = get_dsp(config);
  assert(dsp->HasInputLevel());
  assert(!dsp->HasOutputLevel());
}
}; // namespace test_get_dsp