/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackCrackingBase.hpp"

AttackCrackingBase::AttackCrackingBase(const ConfigTask& config, Directory& directory, const char* attack_mode) : AttackBase(config), directory_(directory) {

  /** For benchmark only allowed attack_mode value is 3 */
  findAndAddRequired("attack_mode", "-a", attack_mode);

  if (!config_.find("attack_submode", attack_submode_))
  RunnerUtils::runtimeException("attack_submode is missing in config");
  
}

void AttackCrackingBase::addSpecificArguments() {

  addRequiredFile("data");

  findAndAddOptional(ConfigTask::START_INDEX, "--skip");
  findAndAddOptional(ConfigTask::HC_KEYSPACE, "--limit");
  findAndAddOptional(ConfigTask::GENERATE_RANDOM_RULES, "--generate-rules");
  findAndAddOptional(ConfigTask::HWMON_TEMP_ABORT, "--hwmon-temp-abort");

  std::string value;
  if (config_.find(ConfigTask::OPTIMIZED, value) && value == "1")
    addArgument("--optimized-kernel-enable");

  if (config_.find(ConfigTask::HEX_DICT, value) && value == "1")
    addArgument("--hex-wordlist");

  addArgument("--status-timer="+RunnerUtils::toString(HashcatConstant::ProgressPeriod));

  addArgument("--outfile");
  addArgument(output_file_);

  addArgument("--outfile-format=" + HashcatConstant::OutputFormat);
  addArgument("--quiet");
  addArgument("--status");
  addArgument("--status-json");
  addArgument("--restore-disable");
  addArgument("--potfile-disable");
  addArgument("--logfile-disable");
}

void AttackCrackingBase::addRequiredFile(const std::string& file_name) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name : " + file_name);

  File file;
  if (!directory_.find(file_name, file)) {
    RunnerUtils::runtimeException("Missing hashcat required file " + file_name);
  }

  addArgument(file.getRelativePath());
}

void AttackCrackingBase::addOptionalFile(const std::string& file_name, const std::string& argument) {

  Logging::debugPrint(Logging::Detail::CustomOutput, " : file_name = " + file_name);

  File file;

  if (directory_.find(file_name, file)) {
    addArgument(argument);
    addArgument(file.getRelativePath());
  }
}
