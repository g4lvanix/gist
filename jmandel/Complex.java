public class Complex
{
    private double real;
    private double imag;
    
    /**
    * Creates a new Complex number object z
    */
    public Complex()
    {
        this(0.0,0.0);
    }

    /**
    * Creates a new Complex number object z
    * @param real   real part of z
    * @param imag   imaginary part of z
    */
    public Complex(double real, double imag)
    {
        this.real = real;
        this.imag = imag;
    }
    
    /**
    * Returns the real part of the complex number
    * @return Re(z)
    */
    public double re()
    {
        return real;
    }

    /**
    * Returns the imaginary part of the complex number
    * @return Im(z)
    */
    public double im()
    {
        return imag;
    }

    /**
    * Returns the square of the magnitude of the complex number
    */
    public double mag2()
    {
        return ( (real*real) + (imag*imag)  );
    }
    
    /** 
    * Returns a new Complex number with value z^2
    * @return Complex(Re(z^2), Im(z^2))
    */
    public Complex square()
    {
        double tr,ti;
        Complex c;
        
        tr = (real*real) - (imag*imag);
        ti = 2*real*imag;
        
        c = new Complex(tr,ti);
        
        return c;
    }

    /**
    * Calculates the argument (angle) of the Complex number
    * measured counter clockwise from the positive real axis
    * @return arg(z)    0 <= arg(z) < 2*pi
    */
    public double arg()
    {
        double arg;

        arg = Math.atan2(imag, real);

        if(arg < 0){
            arg += 2*Math.PI;
        }
        
        if(arg == 2*Math.PI){
            arg = 0;
        }

        return arg;
    }

    /**
    * Return the a String representation of the Complex number
    * @return a String of the complex number in format x+jy
    */
    public String toString()
    {
        String s;
        s = Double.toString(real);
        
        if (imag >= 0){
            s += "+" + Double.toString(imag);
        } else {
            s += Double.toString(imag);
        }
        
        s += "j";
        
        return s;       
    }

    @Override
    public boolean equals(Object obj)
    {
        if(obj == null){ 
            return false;
        }
        
        if(!(obj instanceof Complex)){ 
            return false;
        }

        Complex that = (Complex) obj;

        if(that.real != this.real)
        {
            return false;
        }

        if(that.imag != this.imag)
        {
            return false;
        }

        return true;
    }
}
