import java.io.FileWriter;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.jz.digital.service.DigitalTwinService;
import com.jz.digital.dto.*;
import com.jz.digital.service.impl.DigitalTwinServiceImpl;


class MsgExchanger{
    public static void main( String[] args)
    {
        System.out.print("hello world");
        System.out.print(System.getProperty("java.classpath"));

        DigitalTwinService dt_service = new DigitalTwinServiceImpl();
        System.out.print("digitial twin service object ok!");
        try{
            List<QuayRTGDTO> ls_rtg = dt_service.getRTG();
        }
        catch( Exception e)
        {
            System.out.print(e.getMessage());

        }

        
        

        System.out.print("==over===");

    }

}
