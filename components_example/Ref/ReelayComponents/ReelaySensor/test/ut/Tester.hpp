// ======================================================================
// \title  ReelaySensor/test/ut/Tester.hpp
// \author Roman Riesen
// \brief  hpp file for ReelaySensor test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Ref/ReelaySensor/ReelaySensor.hpp"

namespace Ref {

  class Tester :
    public ReelaySensorGTestBase
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

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_eventOut
      //!
      void from_eventOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          const Ref::ReelayEvent &reelay_event 
      );

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
      ReelaySensor component;

  };

} // end namespace Ref

#endif
