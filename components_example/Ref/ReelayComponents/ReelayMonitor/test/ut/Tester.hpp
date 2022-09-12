// ======================================================================
// \title  ReelayMonitor/test/ut/Tester.hpp
// \author root
// \brief  hpp file for ReelayMonitor test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Ref/ReelayMonitor/ReelayMonitor.hpp"

namespace Ref {

  class Tester :
    public ReelayMonitorGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:

      //! Construct object Tester
      //!
      Tester();

      //! Destroy object Tester
      //!
      ~Tester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! To do
      //!
      void toDo();

    protected:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_verdictOut
      //!
      void from_verdictOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          bool verdict, 
          U32 property_id 
      ) override;

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      ReelayMonitor component;

  };

} // end namespace Ref

#endif
