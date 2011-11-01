package fdk.mod;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.Charset;

import fdk.cfg.CFG;
import fdk.cfg.CFGFile;
import fdk.lst.BasicEntryMaker;
import fdk.lst.LST;

public class Mod
{

    private File    m_workDir = null;
    private CFGFile m_modConfig;
    private CFGFile m_falloutCfg;

    public Mod(File root, boolean create) throws FileNotFoundException,
            IOException
    {
        initFalloutCfg(root, create);
        initWorkDir(root, create);
        initConfig(create);
    }

    private void initFalloutCfg(File root, boolean create)
            throws FileNotFoundException, IOException
    {
        try
        {
            m_falloutCfg = new CFGFile(new File(root, "fallout2.cfg"));
        }
        catch (FileNotFoundException e)
        {
            if (create)
            {
                m_falloutCfg = new CFGFile(new File(root, "fallout2.cfg"), true);
                m_falloutCfg.read(getClass()
                        .getResourceAsStream("fallout2.cfg"));
            }
            else
            {
                throw new FileNotFoundException(
                        "Configuration file fallout2.cfg not found");
            }
        }
    }

    protected void initWorkDir(File root, boolean create)
            throws FileNotFoundException
    {

        String wd = m_falloutCfg.getValue("system.master_patches");
        if (wd != null)
            m_workDir = new File(root, wd);

        if (m_workDir == null)
        {
            m_workDir = new File(root, "data");
        }

        if (!m_workDir.exists())
        {
            if (create)
            {
                m_workDir.mkdirs();
            }
            else
            {
                throw new FileNotFoundException("Working dir " + m_workDir
                        + " not found");
            }
        }
    }

    protected void initConfig(boolean create) throws FileNotFoundException,
            IOException
    {
        try
        {
            m_modConfig = new CFGFile(new File(m_workDir, "mod.cfg"), create);
        }
        catch (FileNotFoundException e)
        {
            throw new FileNotFoundException(
                    "Configuration file mod.cfg not found");
        }
    }

    protected void initSubMods() throws IOException
    {
        try
        {
            int submod = Integer.parseInt(m_modConfig.getValue("ActiveSubMod"));
            LST lst = new LST(new FileInputStream(new File(
                    m_workDir,
                    "submods.lst")), Charset.defaultCharset(),
                    new BasicEntryMaker());
            String submodCfg = lst.get(submod).getValue();
            CFG cfg = new CFGFile(new File(m_workDir, submodCfg));
            cfg.getValue("Sfall");
        }
        catch (NumberFormatException e)
        {
            // TODO: handle exception
        }
        catch (FileNotFoundException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        catch (IndexOutOfBoundsException e)
        {
            // TODO: handle exception
        }
    }

    public CFG getConfig()
    {
        return m_modConfig;
    }

    public File getWorkDir()
    {
        return m_workDir;
    }
}
