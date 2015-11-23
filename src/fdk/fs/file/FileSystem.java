package fdk.fs.file;

import java.io.*;

import fdk.fs.*;

public class FileSystem implements IFileSource {

	private File m_root;

	public FileSystem(File root) throws Exception {
		if (root.exists() && !root.isDirectory())
			throw new Exception("Root must be directory ");
		m_root = root;
	}

	public InputStream getFile(FileSystemFile file) throws FileNotFoundException {
		return new FileInputStream(file.getFile());
	}

	@Override
	public InputStream getFile(String fname) throws IOException {
		File open = new File(m_root.getCanonicalPath() + "\\" + fname);
		if (open.exists() && open.isFile())
			return new FileInputStream(open);

		return null;
	}

	@Override
	public InputStream getFile(IFileEntry fent) throws IOException {
		if (fent instanceof FileSystemFile) {
			return getFile((FileSystemFile) fent);
		} else {
			return getFile(fent.getFullName());
		}
	}

	@Override
	public IFileEntry getRoot() {
		return new FileSystemDir(m_root, m_root);
	}
}
