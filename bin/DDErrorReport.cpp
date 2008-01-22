#include <iostream>
#include <fstream>

#include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/Parser/interface/FIPConfiguration.h"
#include "DetectorDescription/RegressionTest/interface/DDErrorDetection.h"
#include <boost/shared_ptr.hpp>
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PluginManager/interface/ProblemTracker.h"
#include "FWCore/Utilities/interface/Presence.h"
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;
namespace DD { } using namespace DD;

int main(int argc, char *argv[])
{
  std::string const kProgramName = argv[0];
  int rc = 0;

  // Copied from example stand-alone program in Message Logger July 18, 2007
  try {

    // A.  Instantiate a plug-in manager first.
   edm::AssertHandler ah;

   // B.  Load the message service plug-in.  Forget this and bad things happen!
   //     In particular, the job hangs as soon as the output buffer fills up.
   //     That's because, without the message service, there is no mechanism for
   //     emptying the buffers.
    boost::shared_ptr<edm::Presence> theMessageServicePresence;
    theMessageServicePresence = boost::shared_ptr<edm::Presence>(edm::PresenceFactory::get()->
      makePresence("MessageServicePresence").release());

    // C.  Manufacture a configuration and establish it.
    std::string config =
      "process x = {"
      "service = MessageLogger {"
        "untracked vstring destinations = {'infos.mlog','warnings.mlog'}"
        "untracked PSet infos = {"
          "untracked string threshold = 'INFO'"
          "untracked PSet default = {untracked int32 limit = 1000000}"
          "untracked PSet FwkJob = {untracked int32 limit = 0}"
        "}"
        "untracked PSet warnings = {"
          "untracked string threshold = 'WARNING'"
          "untracked PSet default = {untracked int32 limit = 1000000}"
        "}"
        "untracked vstring fwkJobReports = {'FrameworkJobReport.xml'}"
        "untracked vstring categories = {'FwkJob'}"
        "untracked PSet FrameworkJobReport.xml = {"
          "untracked PSet default = {untracked int32 limit = 0}"
          "untracked PSet FwkJob = {untracked int32 limit = 10000000}"
        "}"
      "}"
      "service = JobReportService{}"
      "service = SiteLocalConfigService{}"
      "}";


    boost::shared_ptr<std::vector<edm::ParameterSet> > pServiceSets;
    boost::shared_ptr<edm::ParameterSet>          params_;
    edm::makeParameterSets(config, params_, pServiceSets);

    // D.  Create the services.
    edm::ServiceToken tempToken(edm::ServiceRegistry::createSet(*pServiceSets.get()));

    // E.  Make the services available.
    edm::ServiceRegistry::Operate operate(tempToken);

    // END Copy from example stand-alone program in Message Logger July 18, 2007

    cout << "initialize DDL parser" << endl;
    DDLParser* myP = DDLParser::instance();

    //     cout << "about to set configuration" << endl;
    /* The configuration file tells the parser what to parse.
       The sequence of files to be parsed does not matter but for one exception:
       XML containing SpecPar-tags must be parsed AFTER all corresponding
       PosPart-tags were parsed. (Simply put all SpecPars-tags into seperate
       files and mention them at end of configuration.xml. Functional SW 
       will not suffer from this restriction).
    */  

    cout << "about to start parsing" << endl;
    string configfile("configuration.xml");
    if (argc==2) {
      configfile = argv[1];
    }
    //    Use the File-In-Path configuration document provider.
    FIPConfiguration fp;
    fp.readConfig(configfile);
    int parserResult = myP->parse(fp);
    cout << "done parsing" << std::endl;
    cout.flush();
    if (parserResult != 0) {
      cout << " problem encountered during parsing. exiting ... " << endl;
      exit(1);
    }
    cout << " parsing completed" << endl;
  
    cout << endl << endl << "Start checking!" << endl << endl;
 
    DDErrorDetection ed;
    ed.scan();
    ed.report(cout);

    return 0;
  
  }
  catch (DDException& e)
    {
      cerr << "DDD-PROBLEM:" << endl 
	   << e << endl;
    }  
  //  Deal with any exceptions that may have been thrown.
  catch (cms::Exception& e) {
    std::cout << "cms::Exception caught in "
      	      << kProgramName
	      << "\n"
	      << e.explainSelf();
    rc = 1;
  }
  catch (std::exception& e) {
    std::cout << "Standard library exception caught in "
      	      << kProgramName
	      << "\n"
	      << e.what();
    rc = 1;
  }
  catch (...) {
    std::cout << "Unknown exception caught in "
      	      << kProgramName;
    rc = 2;
  }

  return rc;
  
}
