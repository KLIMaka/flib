package fdk.fs.override;

import java.util.*;

import fdk.fs.IFileEntry;

public class OverrideNode implements IFileEntry {

	private List<IFileEntry> m_overrides = new ArrayList<IFileEntry>();

	public OverrideNode(IFileEntry ent) {
		m_overrides.add(ent);
	}

	public IFileEntry top() {
		return m_overrides.get(m_overrides.size() - 1);
	}

	protected IFileEntry bottom() {
		return m_overrides.get(0);
	}

	public void addOverride(IFileEntry ent) {
		if (isDirectory() == ent.isDirectory())
			m_overrides.add(ent);
	}

	protected List<IFileEntry> getOverrides() {
		return m_overrides;
	}

	@Override
	public String getName() {
		return top().getName();
	}

	@Override
	public String getFullName() {
		return top().getFullName();
	}

	@Override
	public int getSize() {
		return top().getSize();
	}

	@Override
	public boolean isDirectory() {
		return false;
	}

	@Override
	public IFileEntry[] getSub() {
		return null;
	}

}
