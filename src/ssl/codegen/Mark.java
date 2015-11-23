package ssl.codegen;

public class Mark {
	int m_cluster;
	int m_offset;

	public Mark(int c, int o) {
		m_cluster = c;
		m_offset = o;
	}

	public int getCluster() {
		return m_cluster;
	}

	public int getOffset() {
		return m_offset;
	}

}
