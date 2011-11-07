package fdk.cfg;

public class CFGEntry implements Cloneable {

    private String  m_category;
    private String  m_name;
    private String  m_value;
    private String  m_precomment;
    private String  m_postcomment;
    private boolean m_enabled;

    public CFGEntry(String category, String name, String value, String precomment, String postcomment, boolean enabled) {
        m_category = category;
        m_name = name;
        m_value = value;
        m_precomment = precomment;
        m_postcomment = postcomment;
        m_enabled = enabled;
    }

    public String getCategory() {
        return m_category;
    }

    public String getName() {
        return m_name;
    }

    public String getValue() {
        return m_value;
    }

    public void SetValue(String val) {
        m_value = val;
    }

    @Override
    public String toString() {
        return m_name;
    }

    public String getPrecomment() {
        return m_precomment;
    }

    public String getPostcomment() {
        return m_postcomment;
    }

    public boolean isEnabled() {
        return m_enabled;
    }

    public void setEnabled(boolean enabled) {
        m_enabled = enabled;
    }

    @Override
    public CFGEntry clone() throws CloneNotSupportedException {
        return (CFGEntry) super.clone();
    }

}
