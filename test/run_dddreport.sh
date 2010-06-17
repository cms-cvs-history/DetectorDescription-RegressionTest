#!/bin/bash

test=testdddreport

function die { echo Failure $1: status $2 ; exit $2 ; }

pushd ${LOCAL_TMP_DIR}
  export mecpath=${PATH}
  export PATH=./:${PATH}
  echo "DONE SETTING PATHS"
  cp ${LOCAL_TOP_DIR}/src/GeometryReaders/XMLIdealGeometryESSource/test/testdddreport.sh .
  cp ${LOCAL_TOP_DIR}/test/${SCRAM_ARCH}/DDErrorReport .
  echo "DONE WITH COPY AND ABOUT TO SHOW PATH"
  echo $PATH
  echo "DONE SHOWING PATH"
  which DDErrorReport
  echo ${test}IdealGeometry ---------------------------------------------------------------
  ./testdddreport.sh || die "testdddreport.sh cmsIdealGeometryXML_cfi.py" $?
  echo ${test}ExtendedGeometry ---------------------------------------------------------------
  ./testdddreport.sh cmsExtendedGeometryXML_cfi.py || die "testdddreport.sh cmsExtendedGeometryXML_cfi.py" $?
  echo ${test}ExtendedGeometryLiMax ---------------------------------------------------------------
  ./testdddreport.sh cmsExtendedGeometryLiMaxXML_cfi.py || die "testdddreport.sh cmsExtendedGeometryLiMaxXML_cfi.py" $?
  export PATH=${mecpath}
popd

exit 0
