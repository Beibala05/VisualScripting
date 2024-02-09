#pragma once

#include <QString>

namespace nodes_styles 
{

    const QString digit_node_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid green;"
        "border-radius: 5px;"
        "color: white;";

    const QString bool_node_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid red;"
        "border-radius: 5px;"
        "color: white;";

    const QString string_node_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid pink;"
        "border-radius: 5px;"
        "color: white;";

    const QString any_node_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 2px solid blue;"
        "border-radius: 5px;"
        "color: white;";

    const QString digit_label_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 2px solid green;"
        "border-radius: 10px;"
        "color: white;";

    const QString string_label_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 2px solid pink;"
        "border-radius: 10px;"
        "color: white;";
        
    const QString bool_label_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 2px solid red;"
        "border-radius: 10px;"
        "color: white;";

    const QString btn_current_begin_style =
        "background-color: rgba(255, 255, 0, 0.1);"
        "border-top-left-radius: 10px;"
        "border-bottom-left-radius: 10px;"
        "border-top-right-radius: 0px;"
        "border-bottom-right-radius: 0px;"
        "border: 2px solid yellow;"
        "color: white;";

    const QString btn_begin_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border-top-left-radius: 10px;"
        "border-bottom-left-radius: 10px;"
        "border-top-right-radius: 0px;"
        "border-bottom-right-radius: 0px;"
        "color: white;";

    const QString btn_end_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border-top-right-radius: 10px;"
        "border-bottom-right-radius: 10px;"
        "border-top-left-radius: 0px;"
        "border-bottom-left-radius: 0px;"
        "color: white;";

    const QString set_btn_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 0px solid green;"
        "color: white;";

    const QString digit_input_output_style =
        "background-color: green;"
        "border: 2px solid green;"
        "border-radius: 7;"
        "color: white;";

    const QString bool_input_output_style =
        "background-color: red;"
        "border: 2px solid red;"
        "border-radius: 7;"
        "color: white;";

    const QString string_input_output_style =
        "background-color: pink;"
        "border: 2px solid pink;"
        "border-radius: 7;"
        "color: white;";

    const QString any_input_output_style =
        "background-color: blue;"
        "border: 2px solid blue;"
        "border-radius: 7;"
        "color: white;";

    const QString node_name_style =
        "background-color: rgba(216, 216, 216, 0);"
        "border: 2px solid rgba(216, 216, 216, 0);"
        "border-radius: 0;"
        "color: white;";

    const QString var_name_style =
        "background-color: rgba(216, 216, 216, 0);"
        "border: 2px solid rgba(216, 216, 216, 0);"
        "border-radius: 0;"
        "color: white;";

    const QString bool_check_box_style =
        "background-color: rgba(216, 216, 216, 0);"
        "border: 2px solid rgba(216, 216, 216, 0);"
        "border-radius: 0;"
        "color: white;";
}

namespace current_nodes_styles
{
    const QString digit_node_current_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid yellow;"
        "border-radius: 5px;"
        "color: white;";

    const QString bool_node_current_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid yellow;"
        "border-radius: 5px;"
        "color: white;";

    const QString string_node_current_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid yellow;"
        "border-radius: 5px;"
        "color: white;";

    const QString any_node_current_style =
        "background-color: rgba(0, 0, 0, 0.1);"
        "border: 3px solid yellow;"
        "border-radius: 5px;"
        "color: white;";
}

namespace windows_styles
{
    const QString main_window_style =
        "background-color: #22222e;"
        "color: white;";

    const QString get_vars_window_style =
        "background-color: #22222e;"
        "border: 2px solid green;"
        "color: white;";

    const QString dialog_window_label_style = 
        "background-color: #22222e;"
        "border: 2px solid green;"
        "color: white;";

    const QString dialog_window_btn_green_style =
        "color: white;"
        "background-color: #22222e;"
        "border: 2px solid green;"
        "color: white;";

    const QString dialog_window_btn_red_style =
        "color: white;"
        "background-color: #22222e;"
        "border: 2px solid red;"
        "color: white;";
}

namespace icons
{
    const QString save_icon         = ":/icons/res/icons/save.png";
    const QString compile_icon      = ":/icons/res/icons/compile.png";
    const QString run_icon          = ":/icons/res/icons/run.png";
    const QString terminal_icon     = ":/icons/res/icons/terminal.png";
    const QString clear_icon        = ":/icons/res/icons/clear.png";
    const QString code_icon         = ":/icons/res/icons/code.png";
    const QString compiler_icon     = ":/icons/res/icons/compiler.png";
    const QString good              = ":/icons/res/icons/good.png";
    const QString bad               = ":/icons/res/icons/err.png";
    const QString update_icon       = ":/icons/res/icons/update.png";
}
