package fdk.fs;

import java.io.*;

public interface IFileSource {

	public InputStream getFile(String fname) throws IOException;

	public InputStream getFile(IFileEntry fent) throws IOException;

	public IFileEntry getRoot();

}
