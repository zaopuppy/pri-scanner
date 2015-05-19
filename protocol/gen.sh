#!/bin/bash


main()
{
  # /Volumes/Data/source/google/protobuf-2.5.0/out/bin/protoc \
  protoc \
    --cpp_out=../gw \
    *.proto
}

cd $(dirname $0)
main "$@"

