package fdk.fs;

import java.io.IOException;
import java.io.InputStream;

public interface IFileSource
{

    public InputStream getFile(String fname) throws IOException;

    public InputStream getFile(IFileEntry fent) throws IOException;

    public IFileEntry getRoot();

}
