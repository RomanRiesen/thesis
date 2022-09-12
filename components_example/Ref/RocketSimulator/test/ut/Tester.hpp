// ======================================================================
// \title  RocketSimulator/test/ut/Tester.hpp
// \author Roman Riesen
// \brief  hpp file for RocketSimulator test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Ref/RocketSimulator/RocketSimulator.hpp"

namespace Ref {

  class Tester :
    public RocketSimulatorGTestBase
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

      //! Handler for from_exhaustVelocity
      //!
      void from_exhaustVelocity_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 value 
      );

      //! Handler for from_height
      //!
      void from_height_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 value 
      );

      //! Handler for from_rocket_t
      //!
      void from_rocket_t_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 value 
      );

      //! Handler for from_speedD
      //!
      void from_speedD_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 value 
      );

      //! Handler for from_speedI
      //!
      void from_speedI_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 value 
      );

      //! Handler for from_stage
      //!
      void from_stage_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          const Ref::RocketStage &stage 
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
      RocketSimulator component;

  };

} // end namespace Ref

#endif
