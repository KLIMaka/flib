package ssl.generator.symbols.variables;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.symbols.constants.Constant;
import ssl.generator.symbols.constants.IntConstant;

public class LocalVariable extends Variable
{

    private String      m_name;
    private Constant<?> m_default = null;
    private int         m_index;

    public LocalVariable(String name, Constant<?> def)
    {
        m_name = name;
        m_default = def;
        if (m_default == null)
        {
            m_default = new IntConstant("", 0);
        }
    }

    public int getIndex()
    {
        return m_index;
    }

    public void setIndex(int index)
    {
        m_index = index;
    }

    @Override
    protected void writeSelf(ByteStream stream)
    {
        stream.write(OP.INTOP);
        stream.write(getIndex());
    }

    @Override
    protected void writeSet(ByteStream stream)
    {
        stream.write(OP.STORE);
    }

    @Override
    protected void writeGet(ByteStream stream)
    {
        stream.write(OP.FETCH);
    }

    @Override
    public String getName()
    {
        return m_name;
    }

    public Constant<?> getDefault()
    {
        return m_default;
    }
}
