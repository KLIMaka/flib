package fdk.fs.override;

import java.io.*;
import java.util.*;

import fdk.fs.*;

public class OverridableFS implements IFileSource {

	private List<IFileSource> m_overrides;

	public OverridableFS(List<IFileSource> list) {
		m_overrides = list;
	}

	public OverridableFS(IFileSource src) {
		m_overrides = new ArrayList<IFileSource>();
		m_overrides.add(src);
	}

	public void addOverride(IFileSource src) {
		m_overrides.add(src);
	}

	@Override
	public InputStream getFile(String fname) throws IOException {
		for (int i = m_overrides.size() - 1; i >= 0; i--) {
			InputStream stream = m_overrides.get(i).getFile(fname);
			if (stream == null) {
				continue;
			} else {
				return stream;
			}
		}
		return null;
	}

	@Override
	public InputStream getFile(IFileEntry fent) throws IOException {
		if (fent instanceof OverrideNode) {
			fent = ((OverrideNode) fent).top();
		}

		for (int i = m_overrides.size() - 1; i >= 0; i--) {
			InputStream stream = m_overrides.get(i).getFile(fent);
			if (stream == null) {
				continue;
			} else {
				return stream;
			}
		}
		return null;
	}

	@Override
	public IFileEntry getRoot() {
		OverrideDir root = new OverrideDir(m_overrides.get(0).getRoot());
		for (int i = 1; i < m_overrides.size(); i++) {
			root.addOverride(m_overrides.get(i).getRoot());
		}
		return root;
	}

}
