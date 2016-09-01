package github.worm;

import java.util.Arrays;

public class Interceptor
{
    private String str;

    private int index = 0;

    private char[] operators;

    private double[] values;

    public Interceptor(String s)
    {
        str = s;
    }

    public void preprocess()
    {
        String regexp = "[\\*, \\+, \\(, \\), \\/, -]{1}";
        String sps[] = str.split(regexp);
        values = new double[sps.length];
        for (int idx = 0; idx < sps.length; idx++)
        {
            String str = sps[idx].trim();
            if (str.isEmpty())
            {
                values[idx] = Double.NaN;
                continue;
            }
            if (str.contains("."))
            {
                values[idx] = Double.parseDouble(str);
            } else
            {
                values[idx] = Double.parseDouble(str + ".0");
            }
        }
        operators = str.replaceAll("[0-9, \\., \\s]+", "").toCharArray();
    }

    public Express walk(Express exp)
    {
        if (operators.length <= index)
            return exp;
        char op = operators[index];

        if (op == '(')
        {
            index++;
            if (null != exp)
            {
                exp.addElement(walk(null));
            }
        }
        else if (op == '+' || op == '-' || op == '*' || op == '/' || op == '^')
        {

            Express.Func func = op == '+' ? Express.Func.Plus
                    : op == '-' ? Express.Func.Minus
                            : op == '*' ? Express.Func.Multi
                                    : op == '/' ? Express.Func.Div
                                            : Express.Func.Pow;
            double right = values[index+1];
            double def = op == '-' || op == '+' ? 0.0 : 1.0;
            if (Double.isNaN(right)) {
                right = def;
            }
            if (null == exp)
            {
                double left = values[index];
                if (Double.isNaN(left)) {
                    left = def;
                }
                exp = new Express(func, new Express(left), new Express(right));
            } else
            {
                exp = new Express(func, exp, new Express(right));
            }
            index++;
        } else if (op == ')') {
            index ++;
            return exp;
        } else {
            index++;
            System.out.println("Err: " + op);
        }

        return walk(exp);
    }

    public Express process(Express exp)
    {
        if (null == str || str.length() <= index)
        {
            return exp;
        }
        str = str.trim();

        char ch = str.charAt(index);
        if (ch == '(')
        {
            index++;
            Express ex = process(null);
            if (null == ex)
            {
                return null;
            }
            if (exp == null)
            {
                exp = ex;
            } else
            {
                exp.addElement(ex);
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
        {
            Express.Func func = ch == '+' ? Express.Func.Plus
                    : ch == '-' ? Express.Func.Minus
                            : ch == '*' ? Express.Func.Multi : ch == '/' ? Express.Func.Div : Express.Func.Pow;
            if (null == exp)
            {
                exp = new Express(func, new Express(ch == '-' || ch == '+' ? 0.0 : 1.0));
            } else
            {
                exp = new Express(func, exp);
            }
            index++;
            return process(exp);
        } else if ((ch >= '0' && ch <= '9') || ch == '.')
        {
            int start = index;
            while (((ch >= '0' && ch <= '9') || ch == '.') && index < str.length())
            {
                index++;
                ch = str.charAt(index);
            }
            String subs = str.substring(start, index);
            double v = 0;
            if (subs.contains("."))
            {
                v = Double.parseDouble(subs);
            } else
            {
                v = Double.parseDouble(subs + ".0");
            }
            if (null == exp)
            {
                return process(new Express(v));
            }
            exp.addElement(new Express(v));
            return process(exp);
        } else if (ch == ')')
        {
            index++;
            return exp;
        } else
        {
            System.out.println("Err: " + ch);
        }
        return process(exp);
    }
}
