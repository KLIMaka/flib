package fdk.fs.dat;

import fdk.fs.IFileEntry;

public class DatNode implements IFileEntry
{

    private DatNode     m_parent;
    private String      m_name;
    private DatFileInfo m_file;

    public DatNode(String name, DatNode parent, DatFileInfo file)
    {
        setName(name);
        setParent(parent);
        m_file = file;
    }

    public void setName(String name)
    {
        m_name = name;
    }

    public String getName()
    {
        return m_name;
    }

    public void setParent(DatNode parent)
    {
        m_parent = parent;
    }

    public DatNode getParent()
    {
        return m_parent;
    }

    @Override
    public String getFullName()
    {
        if (getParent() != null && !(getParent() instanceof DatRoot))
            return getParent().getFullName() + "\\" + getName();
        return getName();
    }

    public DatFileInfo getFile()
    {
        return m_file;
    }

    @Override
    public int getSize()
    {
        return m_file.getRealSize();
    }

    @Override
    public boolean isDirectory()
    {
        return false;
    }

    @Override
    public IFileEntry[] getSub()
    {
        return null;
    }
}
