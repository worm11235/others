package github.worm;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Express
{
    private List<Express> elements;

    private Double value;

    private Func func;
    
    private boolean updated = false;
    
    public Express (Double val) {
        value = val;
    }
    
    public Express (Func f, Express ...expresses ) {
        this.func = f;
        this.elements = new ArrayList<Express>();
        this.elements.addAll(Arrays.asList(expresses));
    }
    
    public double getValue() {
        return value;
    }
    
    public void addElement (Express e) {
        this.elements.add(e);
    }
    
    public void calculate () {
        if (updated || null == func || null == elements) {
            return;
        }
        
        for (Express ex : elements) {
            ex.calculate();
        }
        
        switch (func) {
            case Plus: {
                value = .0;
                for (Express ex : elements) {
                    value += ex.value;
                }
                break;
            }
            case Minus: {
                value = .0;
                for (Express ex: elements) {
                    if (elements.indexOf(ex) == 0) {
                        value += ex.value;
                    } else {
                        value -= ex.value;
                    }
                }
                break;
            }
            case Multi: {
                value = 1.0;
                for (Express ex: elements) {
                    value *= ex.value;
                }
                break;
            }
            case Div: {
                value = 1.0;
                for (Express ex: elements) {
                    if (elements.indexOf(ex) == 0) {
                        value *= ex.value;
                    } else {
                        value /= ex.value;
                    }
                }
                break;
            }
            case Pow: {
                value = .0;
                for (Express ex : elements) {
                    if (value == 0) {
                        value = ex.value;
                    } else {
                        value = Math.pow(value, ex.value);
                    }
                }
                break;
            }
        }
    }
    
    static enum Func
    {
        Plus, Minus, Multi, Div, Pow;
    }
}
