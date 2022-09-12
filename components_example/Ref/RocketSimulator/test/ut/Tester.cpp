// ======================================================================
// \title  RocketSimulator.hpp
// \author Roman Riesen
// \brief  cpp file for RocketSimulator test harness implementation class
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
      RocketSimulatorGTestBase("Tester", MAX_HISTORY_SIZE),
      component("RocketSimulator")
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
    this->invoke_to_schedIn(0, 0);
    ASSERT_from_speedI_SIZE(1);
    ASSERT_from_speedD_SIZE(1);
    ASSERT_from_stage_SIZE(1);
    ASSERT_from_rocket_t_SIZE(1);
    ASSERT_from_height_SIZE(1);
    ASSERT_from_exhaustVelocity_SIZE(1);
    this->sendCmd_TOGGLE_SIMULATION(0, 0);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE_SIZE(1);
    for (int i = 0; i < 100; i++)
    {
      //this->invoke_to_schedIn(0, 0);
      //ASSERT_from_speedI_SIZE(1);
      //ASSERT_from_speedD_SIZE(1);
      //ASSERT_from_stage_SIZE(1);
      //ASSERT_from_rocket_t_SIZE(1);
      //ASSERT_from_height_SIZE(1);
      //ASSERT_from_exhaustVelocity_SIZE(1);
    }
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_exhaustVelocity_handler(
        const NATIVE_INT_TYPE portNum,
        F32 value
    )
  {
    this->pushFromPortEntry_exhaustVelocity(value);
  }

  void Tester ::
    from_height_handler(
        const NATIVE_INT_TYPE portNum,
        F32 value
    )
  {
    this->pushFromPortEntry_height(value);
  }

  void Tester ::
    from_rocket_t_handler(
        const NATIVE_INT_TYPE portNum,
        F32 value
    )
  {
    this->pushFromPortEntry_rocket_t(value);
  }

  void Tester ::
    from_speedD_handler(
        const NATIVE_INT_TYPE portNum,
        F32 value
    )
  {
    this->pushFromPortEntry_speedD(value);
  }

  void Tester ::
    from_speedI_handler(
        const NATIVE_INT_TYPE portNum,
        F32 value
    )
  {
    this->pushFromPortEntry_speedI(value);
  }

  void Tester ::
    from_stage_handler(
        const NATIVE_INT_TYPE portNum,
        const Ref::RocketStage &stage
    )
  {
    this->pushFromPortEntry_stage(stage);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // cmdIn
    this->connect_to_cmdIn(
        0,
        this->component.get_cmdIn_InputPort(0)
    );

    // schedIn
    this->connect_to_schedIn(
        0,
        this->component.get_schedIn_InputPort(0)
    );

    // cmdRegOut
    this->component.set_cmdRegOut_OutputPort(
        0,
        this->get_from_cmdRegOut(0)
    );

    // cmdResponseOut
    this->component.set_cmdResponseOut_OutputPort(
        0,
        this->get_from_cmdResponseOut(0)
    );

    // exhaustVelocity
    this->component.set_exhaustVelocity_OutputPort(
        0,
        this->get_from_exhaustVelocity(0)
    );

    // height
    this->component.set_height_OutputPort(
        0,
        this->get_from_height(0)
    );

    // rocket_t
    this->component.set_rocket_t_OutputPort(
        0,
        this->get_from_rocket_t(0)
    );

    // speedD
    this->component.set_speedD_OutputPort(
        0,
        this->get_from_speedD(0)
    );

    // speedI
    this->component.set_speedI_OutputPort(
        0,
        this->get_from_speedI(0)
    );

    // stage
    this->component.set_stage_OutputPort(
        0,
        this->get_from_stage(0)
    );

    // timeGetOut
    this->component.set_timeGetOut_OutputPort(
        0,
        this->get_from_timeGetOut(0)
    );

    // tlmOut
    this->component.set_tlmOut_OutputPort(
        0,
        this->get_from_tlmOut(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

} // end namespace Ref
