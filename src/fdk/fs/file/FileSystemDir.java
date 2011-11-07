package fdk.fs.file;

import java.io.File;
import java.io.IOException;

import fdk.fs.IFileEntry;

public class FileSystemDir implements IFileEntry {
    private File m_dir;
    private File m_root;

    public FileSystemDir(File dir, File root) {
        m_dir = dir;
        m_root = root;
    }

    @Override
    public String getName() {
        return m_dir.getName();
    }

    @Override
    public String getFullName() {
        try {
            String fullName = m_dir.getCanonicalPath();
            String root = m_root.getCanonicalPath() + "\\";
            fullName = fullName.replace(root, "");
            return fullName;
        } catch (IOException e) {
            return m_dir.getPath();
        }
    }

    @Override
    public int getSize() {
        return 0;
    }

    @Override
    public boolean isDirectory() {
        return true;
    }

    @Override
    public IFileEntry[] getSub() {
        File files[] = m_dir.listFiles();
        if (files == null) {
            files = new File[0];
        }

        IFileEntry ents[] = new IFileEntry[files.length];
        int i = 0;
        for (File f : files) {
            try {
                if (f.isFile())
                    ents[i++] = new FileSystemFile(f, m_root);
                else ents[i++] = new FileSystemDir(f, m_root);
            } catch (Exception e) {
                continue;
            }
        }

        return ents;
    }
}
