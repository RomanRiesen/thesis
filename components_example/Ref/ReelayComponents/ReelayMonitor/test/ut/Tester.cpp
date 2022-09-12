// ======================================================================
// \title  ReelayMonitor.hpp
// \author root
// \brief  cpp file for ReelayMonitor test harness implementation class
// ======================================================================

#include "Tester.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10
#define QUEUE_DEPTH 10

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      ReelayMonitorGTestBase("Tester", MAX_HISTORY_SIZE),
      component("ReelayMonitor")
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
    ReelayEvent ev("{\"time\": 5, \"speed\": 20.5, \"mode\": \"START\"}");
    ReelayEvent ev2("{\"time\": 5, \"speed\": 19.5, \"mode\": \"START\"}");
    this->invoke_to_newProperty(0, ReelayProperty("{lights_on: true, speed > 20.0, mode: \"START\"}", 0));
    this->invoke_to_eventIn(0, ev);
    this->invoke_to_eventIn(0, ev2);
    //this->component.doDispatch();//somehow makes the program not terminate anymore... :(
    ASSERT_from_verdictOut_SIZE(2);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester::from_verdictOut_handler(
      const NATIVE_INT_TYPE portNum, /*!< The port number*/
      bool verdict, 
      U32 property_id 
  ) {
    this->pushFromPortEntry_verdictOut(verdict, property_id);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // eventIn
    this->connect_to_eventIn(
        0,
        this->component.get_eventIn_InputPort(0)
    );

    // newProperty
    this->connect_to_newProperty(
        0,
        this->component.get_newProperty_InputPort(0)
    );

    // removeProperty
    this->connect_to_removeProperty(
        0,
        this->component.get_removeProperty_InputPort(0)
    );

    // eventOut
    this->component.set_eventOut_OutputPort(
        0,
        this->get_from_eventOut(0)
    );

    // textEventOut
    this->component.set_textEventOut_OutputPort(
        0,
        this->get_from_textEventOut(0)
    );

    // timeGetOut
    this->component.set_timeGetOut_OutputPort(
        0,
        this->get_from_timeGetOut(0)
    );

    // verdictOut
    this->component.set_verdictOut_OutputPort(
        0,
        this->get_from_verdictOut(0)
    );




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
