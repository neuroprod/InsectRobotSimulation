#!/bin/bash
#

set -e

THIS_PATH=$(pwd)
CINDER_PATH=${THIS_PATH}/Cinder
exitWithError() {
  echo "Error:" "$@" >&2
  exit 1
}

checkRequiredTool() {
  for tool in git make cmake ; do
    command -v "${tool}" &> /dev/null || exitWithError "Missing required tool ${tool}"
  done
}

cloneDependenciesIfNeeded() {
  if [ ! -f "${CINDER_PATH}/CMakeLists.txt" ] ; then
    echo "Cinder is not cloned yet, cloning now"
    git clone --recursive https://github.com/oqu/Cinder.git \
      || exitWithError "Cannot clone Cinder"
  fi

  if [ ! -f "${CINDER_PATH}/blocks/Cinder-ImGui/cinderblock.xml" ] ; then
    echo "Cinder-ImGui is not cloned yet, cloning now"
    git clone --recursive https://github.com/paulhoux/Cinder-ImGui.git "${CINDER_PATH}/blocks/Cinder-ImGui" \
      || exitWithError "Cannot clone Cinder-ImGui"
  fi
}

buildCinder() {
  if [ ! -f "${CINDER_PATH}/build/CMakeCache.txt" ] ; then
    echo "Cinder has not been configure, running cmake ..."
    cmake -B "${CINDER_PATH}/build" -DCINDER_DISABLE_IMGUI=1 "${CINDER_PATH}" \
      || exitWithError "Cannot run CMake on Cinder"
  fi
  make -C "${CINDER_PATH}/build" -j4 || exitWithError "Error building Cinder"
}

build() {
  if [ ! -f "${THIS_PATH}/build/CMakeCache.txt" ] ; then
    echo "This project has not been configure, running cmake ..."
    cmake -B "${THIS_PATH}/build" "${THIS_PATH}" \
      || exitWithError "Cannot run CMake on this project"
  fi
  make -C "${THIS_PATH}/build" -j4 || exitWithError "Error building this project"
}

checkRequiredTool
cloneDependenciesIfNeeded
buildCinder
build
