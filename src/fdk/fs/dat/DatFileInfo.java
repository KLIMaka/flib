package fdk.fs.dat;

import fdk.fs.IFileSource;

public class DatFileInfo {

	private String m_fileName;
	private byte m_type;
	private int m_realSize;
	private int m_packedSize;
	private int m_offset;
	private IFileSource m_source;

	public DatFileInfo(String mFileName, byte mType, int mRealSize, int mPackedSize, int mOffset, IFileSource src) {
		m_fileName = mFileName.toLowerCase();
		m_type = mType;
		m_realSize = mRealSize;
		m_packedSize = mPackedSize;
		m_offset = mOffset;
		m_source = src;
	}

	public String getName() {
		return m_fileName;
	}

	public int getSize() {
		return getRealSize();
	}

	public int getRealSize() {
		return m_realSize;
	}

	public int getPackedSize() {
		return m_packedSize;
	}

	public int getOffset() {
		return m_offset;
	}

	public byte getType() {
		return m_type;
	}

	public IFileSource getSource() {
		return m_source;
	}
}
