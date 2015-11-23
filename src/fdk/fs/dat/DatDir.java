package fdk.fs.dat;

import java.util.*;

import fdk.fs.IFileEntry;

public class DatDir extends DatNode {

	private Map<String, DatNode> m_childs = new TreeMap<String, DatNode>(String.CASE_INSENSITIVE_ORDER);

	public DatDir(String name, DatNode parent) {
		super(name, parent, null);
	}

	public DatNode addNode(String path, DatFileInfo file) {
		if (path == "")
			return this;

		String[] parts = path.split("\\\\");

		if (parts.length == 1) {
			DatNode node = new DatNode(path, this, file);
			m_childs.put(path, node);
			return node;
		} else {
			DatDir dir = (DatDir) m_childs.get(parts[0]);
			if (dir == null) {
				dir = new DatDir(parts[0], this);
				m_childs.put(dir.getName(), dir);
			}
			return dir.addNode(path.replaceFirst("[^\\\\]*\\\\", ""), file);
		}
	}

	public DatNode getNode(String path) {
		if (path == "")
			return this;

		String[] parts = path.split("\\\\");

		if (parts.length == 1)
			return m_childs.get(parts[0]);
		else {
			DatNode next = m_childs.get(parts[0]);
			if (next instanceof DatDir) {
				return ((DatDir) next).getNode(path.replaceFirst("[^\\\\]*\\\\", ""));
			}
			return null;
		}
	}

	@Override
	public int getSize() {
		return 0;
	}

	@Override
	public IFileEntry[] getSub() {
		IFileEntry[] list = new IFileEntry[m_childs.size()];
		int i = 0;
		for (IFileEntry ent : m_childs.values())
			list[i++] = ent;
		return list;
	}

	@Override
	public boolean isDirectory() {
		return true;
	}

}
