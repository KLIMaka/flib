package ssl.generator.namespace;

public class Name
{

    private String m_name;
    private int    m_position;

    public Name(String name, int pos)
    {
        m_name = name;
        m_position = pos;
    }

    public int getPosition()
    {
        return m_position;
    }

    public int getLength()
    {
        return m_name.length() + 1 + ((m_name.length() + 1) & 1);
    }

    public String getName()
    {
        return m_name;
    }
}
