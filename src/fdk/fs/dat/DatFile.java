package fdk.fs.dat;

import java.io.*;
import java.util.zip.InflaterInputStream;

import fdk.fs.*;
import fdk.util.*;

public class DatFile implements IFileSource {

	private File m_datFile;
	private RandomAccessFile m_data;
	private DatRoot m_root = new DatRoot();

	public DatFile(String name) throws IOException {
		m_datFile = new File(name);
		m_data = new RandomAccessFile(m_datFile, "r");
		loadFileList();
	}

	private void loadFileList() throws IOException {

		m_data.seek(m_data.length() - 8);
		int treeSize = Utils.toLittleEndian(m_data.readInt());
		int dataSize = Utils.toLittleEndian(m_data.readInt());

		m_data.seek(dataSize - treeSize - 8);

		byte[] buf = new byte[treeSize];
		m_data.read(buf);
		LEDataStream data = new LEDataStream(new ByteArrayInputStream(buf));
		int files = data.readInt();

		for (int i = 0; i < files; i++) {
			String name = data.readString(data.readInt());
			byte type = data.readByte();
			int real = data.readInt();
			int ps = data.readInt();
			int pos = data.readInt();

			m_root.addNode(name, new DatFileInfo(name, type, real, ps, pos, this));
		}
	}

	public InputStream getFile(DatFileInfo file) throws IOException {
		if (file != null && file.getSource() == this) {

			if (file.getSize() == 0)
				return new ByteArrayInputStream(new byte[0]);

			if (file.getPackedSize() == file.getRealSize() && file.getType() == 0) {
				m_data.seek(file.getOffset());
				byte[] buf = new byte[file.getSize()];
				m_data.read(buf);
				return new ByteArrayInputStream(buf);
			}

			m_data.seek(file.getOffset());
			byte[] compressed = new byte[file.getPackedSize()];
			int readed = m_data.read(compressed);
			if (readed != file.getPackedSize()) {
				return null;
			}
			return new InflaterInputStream(new ByteArrayInputStream(compressed));
		}

		return null;
	}

	@Override
	public InputStream getFile(String fname) throws IOException {
		DatNode node = m_root.getNode(fname);
		if (node != null) {
			return getFile(node.getFile());
		}

		return null;
	}

	@Override
	public InputStream getFile(IFileEntry fent) throws IOException {
		if (fent instanceof DatNode) {
			return getFile(((DatNode) fent).getFile());
		} else {
			return getFile(fent.getFullName());
		}
	}

	@Override
	public IFileEntry getRoot() {
		return m_root;
	}

}
