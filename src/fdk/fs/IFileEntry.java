package fdk.fs;

public interface IFileEntry
{

    public String getName();

    public String getFullName();

    public int getSize();

    public boolean isDirectory();

    public IFileEntry[] getSub();
}
