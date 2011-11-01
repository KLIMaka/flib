package ssl.generator;

public class Event
{

    public enum Type
    {
        IN, WHEN
    }

    private Type m_type;

    public Event(Type type)
    {
        m_type = type;
    }
}
