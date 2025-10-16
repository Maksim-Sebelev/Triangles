export module input_stream;

//---------------------------------------------------------------------------------------------------------------

export
enum class input_stream
{
    invalid_input_stream = 0,
    standart_input          , // name `stdin` ia already used
    dat_file_stream         , // for reading from dat files
};

//---------------------------------------------------------------------------------------------------------------
