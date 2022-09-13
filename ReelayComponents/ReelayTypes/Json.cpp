// ======================================================================
// \title  Json.hpp
// \author Roman Riesen
// \brief  hpp file for Json type
// ======================================================================

#include <Ref/ReelayComponents/ReelayTypes/Json.hpp>

namespace ReelayComponents {

      Json::Json() {}

      //! Initialize object RocketSensor
      //!
      void Json::init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      ) {}

      //! Destroy object RocketSensor
      //!
      Json::~Json() {}

      std::string Json::to_std_string() {
        return value.dump();
      }

} // end namespace Ref

#endif
