// ======================================================================
// \title  Json.hpp
// \author Roman Riesen
// \brief  hpp file for Json type
// ======================================================================





#ifndef JSON_FPRIME_TYPE_HPP
#define JSON_FPRIME_TYPE_HPP

#include <Fw/Types/Serializable.hpp>
#include <Fw/Types/SerialStatusEnumAc.hpp>
#include <Ref/ReelayComponents/ReelayMonitor/reelay/include/reelay/json.hpp>

namespace ReelayComponents
{

    /*
        This class is suppposed to implement the Fpp type Json but due to time constraints this is only an outline, 
        to be filled in future work.
    */
    class Json : public Fw::Serializable
    {

        reelay::json value;

    public:
        static const size_t SERIALIZED_SIZE = 256;

        // ----------------------------------------------------------------------
        // Construction, initialization, and destruction
        // ----------------------------------------------------------------------

        //! Construct Json
        //!
        Json();

        //! Initialize Json
        //!
        void init(
            const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
        );

        //! Destroy Json
        //!
        ~Json();

        //const CHAR *operator+=(const CHAR *src);
        const Json &operator+=(const Json &src); 
        bool operator==(const Json &other) const;
        ///bool operator==(const CHAR *other) const;
        bool operator!=(const Json &other) const;
        //bool operator!=(const CHAR *other) const; 
        //Json &operator=(const CHAR *src);
        Json &operator=(const Json &src); 

        //void format(const CHAR *formatString, ...); //!< write formatted string to buffer

        virtual Fw::SerializeStatus serialize(Fw::SerializeBufferBase &buffer) const;                          //!< serialization function
        virtual Fw::SerializeStatus serialize(Fw::SerializeBufferBase &buffer, NATIVE_UINT_TYPE maxLen) const; //!< serialization function
        virtual Fw::SerializeStatus deserialize(Fw::SerializeBufferBase &buffer);                              //!< deserialization function

        std::string to_std_string();
    };

} // end namespace Ref

#endif
