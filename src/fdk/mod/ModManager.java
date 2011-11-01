package fdk.mod;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

public class ModManager
{
    private File m_root;

    public ModManager(File root) throws FileNotFoundException, IOException
    {
        m_root = root;
    }

    public ModManager(String root) throws FileNotFoundException, IOException
    {
        this(new File(root));
    }

    public Mod getDefaultMod(boolean forceCreate)
    {
        try
        {
            return new Mod(m_root, forceCreate);
        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return null;
    }
}
