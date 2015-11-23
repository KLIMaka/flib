package fdk.lst;

import java.io.*;
import java.nio.charset.Charset;
import java.util.*;

public class LST {

	List<IEntry> m_list = new ArrayList<IEntry>();

	public LST() {
	}

	public LST(InputStream is, Charset cs, IEntryMaker maker) throws IOException {
		read(is, maker, cs);
	}

	public int read(InputStream is, IEntryMaker maker, Charset cs) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(is, cs));
		int index = 0;
		for (;;) {
			IEntry ent = maker.create(index, br);
			if (ent == null)
				break;
			add(ent);
			index++;
		}
		return index;
	}

	public void save(OutputStream os, Charset cs) throws IOException {
		BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(os, cs));
		for (int i = 0; i < size(); i++) {
			get(i).write(bw);
			if (i != size())
				bw.write("\r\n");
		}
		bw.close();
	}

	public boolean add(IEntry ent) {
		return m_list.add(ent);
	}

	public IEntry get(int index) {
		return m_list.get(index);
	}

	public int indexOf(IEntry ent) {
		return m_list.indexOf(ent);
	}

	public IEntry remove(int index) {
		return m_list.remove(index);
	}

	public int size() {
		return m_list.size();
	}

	public IEntry[] toArray() {
		return m_list.toArray(new IEntry[m_list.size()]);
	}

}
