/**
 * file  :PaintPanel.java
 * auther:worm
 * date  :2014-1-12
 */
package net.exp.view;

import java.awt.*;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.JPanel;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

/**
 * @author worm
 *
 */
public final class PaintPanel extends JPanel
{
    /**
     * 
     */
    private static final long serialVersionUID = -8582586867253962082L;
    
    private boolean bar = false; //标示是折线图还是条形图
    
    private int margin = 25;
    
    private int yLabelWidth = 10;
    
    private int xLabelHeight = 10;
    
    private int legendHeight = 10;
    
    private int legendLineW = 10;
    
    private int legendTextW = 20;
    
    private int legendW = 40;
    
    private Map<String, Map<String, Double>> data = new HashMap<String, Map<String, Double>>();
    
    private List<String> xLabel = new ArrayList<String>();
    
    private List<String> yLabel = new ArrayList<String>();
    
    private Color[] colorMap = {Color.RED, Color.GREEN, Color.BLUE, Color.CYAN, Color.YELLOW, Color.MAGENTA, Color.ORANGE, Color.PINK};
    
    private double min;
    
    private double max;
    
    private double mind;
    
    private double maxd;
    
    private double step;

    @Override
    public void paintComponent(Graphics gr)
    {
        Graphics2D g2 = (Graphics2D) gr;
        int w = this.getWidth();
        int h = this.getHeight();
        int cw = w - margin * 2 - yLabelWidth;
        int ch = h - margin * 2 - legendHeight - xLabelHeight;
        g2.drawLine(margin + yLabelWidth, margin, margin + yLabelWidth, h
                - margin - legendHeight - xLabelHeight);
        g2.drawLine(margin + yLabelWidth, h - margin - legendHeight
                - xLabelHeight, w - margin, h - margin - legendHeight
                - xLabelHeight);
        for (int i = 0; !bar && i < xLabel.size(); i++)
        {
            g2.drawString(xLabel.get(i), margin + yLabelWidth+(i + 1)
                    * (w - margin * 2 - yLabelWidth) / (xLabel.size() + 1), h
                    - margin-legendHeight);
        }

        calculateStep();

        int stepNum = (int) ((maxd - mind) / step);
        for (int i = 0; i <= stepNum; i++)
        {
            g2.drawString((mind + i * step) + "", margin, (h - margin
                    - legendHeight - xLabelHeight)
                    - i * ch / (stepNum));
            Color clr = g2.getColor();
            g2.setColor(Color.GRAY);
            g2.drawLine(margin + yLabelWidth, (h - margin
                    - legendHeight - xLabelHeight)
                    - i * ch / (stepNum), w - margin, (h - margin
                            - legendHeight - xLabelHeight)
                            - i * ch / (stepNum));
            g2.setColor(clr);
        }
        int index = 0;
        for (Entry<String, Map<String, Double>> dd : data.entrySet())
        {
            Color clr = g2.getColor();
            g2.drawString(dd.getKey(), margin + yLabelWidth + legendW * index
                    + legendLineW, h - margin);
            g2.setColor(colorMap[index % colorMap.length]);
            g2.drawLine(margin + yLabelWidth + legendW * index, h - margin
                    - legendHeight / 2, margin + yLabelWidth + legendW * index
                    + legendLineW, h - margin - legendHeight / 2);

            if (bar)
            {
                int tmp = xLabel.size() * (yLabel.size() + 1) + 1;
                for (int i = 0; i < xLabel.size(); i++)
                {
                    g2.setBackground(g2.getColor());
                    g2.fillRect(margin + yLabelWidth + cw/tmp*(1 + yLabel.size() * i + i + index),
                            (int)((h - margin
                                    - legendHeight - xLabelHeight)
                                    - (dd.getValue().get(xLabel.get(i)) - mind)/step * ch / (stepNum)),
                            cw/tmp, (int)((dd.getValue().get(xLabel.get(i)) - mind)/step*ch/(stepNum)));
                    if (0 == index)
                    {
                        g2.drawString(xLabel.get(i),
                                margin + yLabelWidth + cw/tmp*(1 + yLabel.size() * i + yLabel.size()/2 + i + index), h
                                - margin-legendHeight);
                    }
                }
            } else
            {
                Stroke old = g2.getStroke();
                g2.setStroke(new BasicStroke(2));
                int y1 = 0, y2 = 0;
                for (int i = 0; i < xLabel.size(); i++)
                {
                    if (i == 0)
                    {
                        y1 = (int) (h - margin - legendHeight - xLabelHeight - (dd.getValue().get(xLabel.get(i)) - mind)/step*ch/(stepNum));
                        continue;
                    }
                    y2 = (int) (h - margin - legendHeight - xLabelHeight - (dd.getValue().get(xLabel.get(i)) - mind)/step*ch/(stepNum));
                    g2.drawLine(margin + yLabelWidth+(i)
                            * (w - margin * 2 - yLabelWidth) / (xLabel.size() + 1), y1, margin + yLabelWidth+(i + 1)
                            * (w - margin * 2 - yLabelWidth) / (xLabel.size() + 1), y2);
                    y1=y2;
                }
                g2.setStroke(old);
            }
            
            g2.setColor(clr);
            index++;
        }
    }
    
    private void calculateStep()
    {
        max = 0;
        min = 0;
        boolean s = false;
        for (Map<String, Double> map : data.values())
        {
            for (double v : map.values())
            {
                if (!s)
                {
                    max = v;
                    min = v;
                    s = true;
                    continue;
                }
                max = max > v ? max : v;
                min = min < v ? min : v;
            }
        }
        
        double delta = max - min;
        if (delta == 0)
        {
            delta = max;
        }
        if (delta == 0)
        {
            delta = 1;
        }
        String d = delta + "";
        int magnitude = 0;
        
        if (d.startsWith("0"))
        {
            for (int i = 2; i < d.length(); i ++)
            {
                if (d.charAt(i) != '.' && d.charAt(i) != '0')
                {
                    magnitude = 1 - i;
                    break;
                }
            }
        }
        else
        {
            if (d.contains("."))
            {
                magnitude = d.indexOf(".");
            }
            else
            {
                magnitude = d.length();
            }
        }
        double tmp = magnitude > 0 ? Math.pow(10, magnitude - 1) : Math.pow(10, magnitude);
        double tail = delta % tmp;
        int head = (int) (delta - tail) + 2;
        step = 1;
        if (head > 10)
        {
            step = 2;
        }
        else if (head < 2)
        {
            step = 0.2;
        }
        else if (head < 5)
        {
            step = 0.5;
        }
        
        mind = min - min % step;
        if (mind > min)
        {
            mind -= step;
        }
        
        maxd = max - max % step + step;
        if (maxd < max)
        {
            maxd += step;
        }
    }
    
    public void loadXML(String file) throws DocumentException
    {
        this.loadXML(new File(file));
    }
    
    public void loadXML(File file) throws DocumentException
    {
        data.clear();
        xLabel.clear();
        yLabel.clear();
        SAXReader reader = new SAXReader();
        Document doc = reader.read(file);
        Element root = doc.getRootElement();
        List<Element> days = root.selectNodes("days");
        if (null != days && !days.isEmpty())
        {
            Element day = (Element) days.get(0);
            String str = day.getText();
            for (String s : str.split(","))
            {
                xLabel.add(s);
            }
        }
        
        List<Element> smps = root.selectNodes("sample");
        if (null != smps && !smps.isEmpty())
        {
            for (Element smp : smps)
            {
                yLabel.add(smp.attribute("name").getData() + "");
                List<Element> values = smp.selectNodes("value");
                if (null != values && !values.isEmpty())
                {
                    Map<String, Double> map = new HashMap<String, Double>();
                    for (Element v : values)
                    {
                        map.put(v.attributeValue("day"), Double.parseDouble(v.getText()));
                    }
                    data.put(smp.attributeValue("name"), map);
                }
            }
        }
    }

    /**
     * @return the bar
     */
    public boolean isBar()
    {
        return bar;
    }

    /**
     * @param bar the bar to set
     */
    public void setBar(boolean bar)
    {
        this.bar = bar;
    }

    /**
     * @return the data
     */
    public Map<String, Map<String, Double>> getData()
    {
        return data;
    }

    /**
     * @param data the data to set
     */
    public void setData(Map<String, Map<String, Double>> data)
    {
        this.data = data;
    }

    /**
     * @return the xLabel
     */
    public List<String> getxLabel()
    {
        return xLabel;
    }

    /**
     * @param xLabel the xLabel to set
     */
    public void setxLabel(List<String> xLabel)
    {
        this.xLabel = xLabel;
    }

    /**
     * @return the yLabel
     */
    public List<String> getyLabel()
    {
        return yLabel;
    }

    /**
     * @param yLabel the yLabel to set
     */
    public void setyLabel(List<String> yLabel)
    {
        this.yLabel = yLabel;
    }

}
