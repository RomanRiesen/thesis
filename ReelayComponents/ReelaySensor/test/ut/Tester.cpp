// ======================================================================
// \title  ReelaySensor.hpp
// \author Roman Riesen
// \brief  cpp file for ReelaySensor test harness implementation class
// ======================================================================

#include "Tester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      ReelaySensorGTestBase("Tester", MAX_HISTORY_SIZE),
      component("ReelaySensor")
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
    toDo()
  {
    // TODO
    //construct port type definitions
    //construct test buffers
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_eventOut_handler(
        const NATIVE_INT_TYPE portNum,
        const Ref::ReelayEvent &reelay_event
    )
  {
    this->pushFromPortEntry_eventOut(reelay_event);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // setDataChannelName
    this->connect_to_setDataChannelName(
        0,
        this->component.get_setDataChannelName_InputPort(0)
    );

    // eventOut
    this->component.set_eventOut_OutputPort(
        0,
        this->get_from_eventOut(0)
    );



  // ----------------------------------------------------------------------
  // Connect serial input ports
  // ----------------------------------------------------------------------
    // serialIn
    for (NATIVE_INT_TYPE i = 0; i < 5; ++i) {
      this->connect_to_serialIn(
          i,
          this->component.get_serialIn_InputPort(i)
      );
    }


  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        INSTANCE
    );
  }

} // end namespace Ref
