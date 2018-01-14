mkdir build || (rm -rf build  && mkdir build)
mkdir artifacts || (rm -rf artifacts && mkdir artifacts)

cd artifacts
cmake ..
make
