module ReelayComponents {
      
      #type Json

      @ A Reelay event
      struct ReelayEvent {
        #event_content : Json #alternative design, quite a bit better, but unfinished.
        event_content : string
      }

      struct ReelayProperty {
        property : string
        unique_id : U32 
      }

      @ Used to set either discrete or dense time
      enum ReelayTimeDomain {
        Discrete = 0,
        Dense = 1,
      }

      @ Data type representing the types a reelay data channel can have (T added to avoid namespace conflicts)
      enum ReelayDataType {
         #this `Bottom` is here so the default-constructed enum is not a valid type (increasing likelihood of errors being caught)
         Bottom,
         PolyType,
         Number,
         Bool,
         String,
         ReelayEvent,
      }

}
