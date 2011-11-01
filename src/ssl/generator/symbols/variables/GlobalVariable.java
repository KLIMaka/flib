package ssl.generator.symbols.variables;

import ssl.codegen.ByteStream;
import ssl.generator.OP;
import ssl.generator.symbols.constants.Constant;

public class GlobalVariable extends Variable
{

    private int         m_index;
    private Constant<?> m_default;
    private String      m_name;

    public GlobalVariable(String name, Constant<?> def)
    {
        m_name = name;
        m_default = def;
    }

    public void setIndex(int index)
    {
        m_index = index;
    }

    public Constant<?> getDefault()
    {
        return m_default;
    }

    public int getIndex()
    {
        return m_index;
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
        stream.write(OP.STORE_GLOBAL);
    }

    @Override
    protected void writeGet(ByteStream stream)
    {
        stream.write(OP.FETCH_GLOBAL);
    }

    @Override
    public String getName()
    {
        return m_name;
    }
}
