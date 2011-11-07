package fdk.fs.file;

import java.io.File;
import java.io.IOException;

import fdk.fs.IFileEntry;

public class FileSystemFile implements IFileEntry {

    private File m_file;
    private File m_root;

    public FileSystemFile(File file, File root) throws Exception {
        if (!file.isFile()) throw new Exception("Error");

        m_file = file;
        m_root = root;
    }

    public File getFile() {
        return m_file;
    }

    @Override
    public String getName() {
        return m_file.getName();
    }

    @Override
    public String getFullName() {
        try {
            String fullName = m_file.getCanonicalPath();
            String root = m_root.getCanonicalPath() + "\\";
            fullName = fullName.replace(root, "");
            return fullName;
        } catch (IOException e) {
            return m_file.getPath();
        }
    }

    @Override
    public int getSize() {
        return (int) m_file.length();
    }

    @Override
    public boolean isDirectory() {
        return false;
    }

    @Override
    public IFileEntry[] getSub() {
        return null;
    }

}
