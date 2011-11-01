package fdk.cfg;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class CFGFile extends CFG
{

    private File    m_file;
    private boolean m_forceCreate = false;

    public CFGFile(File file) throws FileNotFoundException, IOException
    {
        m_file = file;
        FileInputStream stream = new FileInputStream(m_file);
        read(stream);
        stream.close();
    }

    public CFGFile(String name) throws FileNotFoundException, IOException
    {
        this(new File(name));
    }

    public CFGFile(String name, boolean forceCreate)
            throws FileNotFoundException,
            IOException
    {
        this(new File(name), forceCreate);
    }

    public CFGFile(File file, boolean forceCreate) throws IOException
    {
        super();
        m_forceCreate = forceCreate;

        if (m_forceCreate == false || file.exists())
        {
            read(new FileInputStream(file));
        }
    }

    private void createFile() throws IOException
    {
        if (!m_file.exists())
        {
            m_file.getParentFile().mkdirs();
            m_file.createNewFile();
        }
    }

    public void save() throws IOException
    {
        if (m_forceCreate)
        {
            createFile();
        }

        FileOutputStream out = new FileOutputStream(m_file);
        write(out);
        out.close();
    }

    public void saveTo(File file) throws IOException
    {
        File tmp = m_file;
        m_file = file;
        save();
        m_file = tmp;
    }

    public String getName()
    {
        return m_file.toString();
    }

    public File getPath(String name)
    {
        File path = new File(getValue(name));
        if (path.isAbsolute())
        {
            return path;
        }

        return new File(m_file.getParentFile(), path.toString());
    }

}
