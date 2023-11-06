#ifndef EVENT_CLASS_TYPE
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
virtual EventType GetEventType() const override { return GetStaticType(); }\
virtual const char* GetName() const override { return #type; }
#endif

#ifndef EVENT_CLASS_CATEGORY
#define EVENT_CLASS_CATEGORY(category) virtual EventCategory GetCategoryFlags() const override {return category; }
#endif