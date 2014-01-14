/**
 * 
 */
package net.exp.view;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JToolBar;

import org.dom4j.DocumentException;

/**
 * @author worm
 *
 */
public class MainPanel extends JFrame implements ActionListener {

    private PaintPanel ppanel;
    
	/**
	 * 
	 */
	private static final long serialVersionUID = 4400040427033950786L;

	public MainPanel ()
	{
		super("Chart Show");
		this.setLayout(new BorderLayout());
		JToolBar tool = new JToolBar();
		this.add(tool, BorderLayout.NORTH);
		tool.setFloatable(false);
		JButton btn = new JButton("Open");
		btn.addActionListener(this);
		tool.add(btn);
		btn = new JButton("Change");
		btn.addActionListener(this);
		tool.add(btn);
		
		ppanel = new PaintPanel();
		ppanel.setBar(true);
		this.add(ppanel, BorderLayout.CENTER);
		setSize(600, 800);
		this.setEnabled(true);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
	}

    @Override
    public void actionPerformed(ActionEvent ae)
    {
        // TODO Auto-generated method stub
        if ("Open".equals(ae.getActionCommand()))
        {
            JFileChooser jfc = new JFileChooser();
            if (JFileChooser.APPROVE_OPTION == jfc.showOpenDialog(this))
            {
                try
                {
                    ppanel.loadXML(jfc.getSelectedFile());
                    repaint();
                } catch (DocumentException e)
                {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
        else if ("Change".equals(ae.getActionCommand()))
        {
            ppanel.setBar(!ppanel.isBar());
            repaint();
        }
    }

}
