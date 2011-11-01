package ssl.generator.namespace;

public class StringSpace extends NameSpace
{

    @Override
    public Name getName(String name)
    {
        for (Name nse : m_space)
        {
            if (nse.getName().equals(name))
            {
                return nse;
            }
        }
        return null;
    }
}