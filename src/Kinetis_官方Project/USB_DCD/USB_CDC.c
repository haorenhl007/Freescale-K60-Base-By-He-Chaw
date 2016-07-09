#include"USB_CDC.h"

/* CDC Global Structures */
CDC_Line_Coding LineCoding;
UINT8 u8CDCState=WAITING_FOR_ENUMERATION;
UINT8 CDC_OUT_Data[CDC_BUFFER_SIZE];


/* USB Variables & Flags */
extern UINT8 gu8USB_Flags; 
extern UINT8 gu8USB_State;              
extern tUSB_Setup *Setup_Pkt;
extern UINT8 gu8EP2_IN_ODD_Buffer[];
extern UINT8 gu8EP3_OUT_ODD_Buffer[];


extern tBDT tBDTtable[];
extern UINT8 gu8Interface;



/**********************************************************/
void CDC_Init(void)
{
    u8CDCState=0;
    
    /* USB Initialization */
    USB_Init();

    
    /* Line Coding Initialization */
    LineCoding.DTERate=LWordSwap(9600);
    LineCoding.CharFormat=0;
    LineCoding.ParityType=0;
    LineCoding.Databits=0x08;

    /* Initialize Data Buffers */
    Buffer_Init(CDC_OUT_Data,CDC_BUFFER_SIZE);
    
}


/**********************************************************/
void CDC_Engine(void)
{
    //UINT16 u8RecData;
    /* control Stage */
    switch(u8CDCState)
    {
        case WAITING_FOR_ENUMERATION:
            /* Wait for USB Enumeration */
            //while(gu8USB_State!=uENUMERATED)
            //{
                //(void)u8CDCState;
                //__RESET_WATCHDOG();
                //USB_FLAGS=USB_ISTAT;
                //USB_ERROR_FLAGS=USB_ERRSTAT;
            //};
            
            
            u8CDCState=0;
            break;

        case SET_LINE_CODING:
            if(FLAG_CHK(EP0,gu8USB_Flags))
            {
                FLAG_CLR(EP0,gu8USB_Flags);
                (void)EP_OUT_Transfer(EP0,(UINT8*)&LineCoding);
                EP_IN_Transfer(EP0,0,0);       
                //vfnSCI1Init();
            }
            break;

        case SET_CONTROL_LINE_STATE:
            EP_IN_Transfer(EP0,0,0);       
            break;
       
    }

    /* Data stage */
/*
    if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            u8RecData=USB_EP_OUT_SizeCheck(EP_OUT);         // Check how many bytes from the PC
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);
            EP_IN_Transfer(EP2,CDC_OUTPointer,2);
            u8RecData=0;
        }
*/
}

/**********************************************************/
UINT8 CDC_InterfaceReq_Handler(void)
{
    UINT8 u8State=uSETUP;
    
    switch(Setup_Pkt->bRequest)
    {        
        case GET_INTERFACE:
            EP_IN_Transfer(EP0,&gu8Interface,1);
            break;

        case GET_LINE_CODING:
            EP_IN_Transfer(EP0,(UINT8*)&LineCoding,7);
            break;

        case SET_LINE_CODING:
            u8CDCState=SET_LINE_CODING;
            u8State=uDATA;
            break;

        case SET_CONTROL_LINE_STATE:
            u8CDCState=SET_CONTROL_LINE_STATE;
            u8State=uSETUP;
            break;


        case LOADER_MODE:
            Buffer_Init(CDC_OUT_Data,CDC_BUFFER_SIZE);
            FLAG_SET(LOADER,gu8USB_Flags);
            CDC_OUT_Data[0]=0xFF;
            break;


    }
    return(u8State);
}


/**********************************************************/
UINT32 LWordSwap(UINT32 u32DataSwap)
{
    UINT32 u32Temp;
    u32Temp= (u32DataSwap & 0xFF000000) >> 24;
    u32Temp+=(u32DataSwap & 0xFF0000)   >> 8;
    u32Temp+=(u32DataSwap & 0xFF00)     << 8;
    u32Temp+=(u32DataSwap & 0xFF)       << 24;
    return(u32Temp);    
}
