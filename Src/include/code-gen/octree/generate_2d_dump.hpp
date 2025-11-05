//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::dump_2d(std::string_view img_name) const
{
#if defined(DOT_OUT_DIR)
    static const std::string dot_out_dir = DOT_OUT_DIR;
#else /* defined(DOT_OUT_DIR) */
#warning "Dot-out dir not given, using default: dot-out/"
    static const std::string dot_out_dir = "dot-out/";
#endif /* defined(DOT_OUT_DIR) */

#if defined(IMG_OUT_DIR)
    static const std::string img_dir     = IMG_OUT_DIR;
#else /* defined(IMG_OUT_DIR) */
#warning "Img out dit not given. using defaul: ../2d-dump/"
    static const std::string img_dir     = "../2d-dump/";
#endif /* defined(IMG_OUT_DIR) */

    create_dir(dot_out_dir);
    create_dir(img_dir    );
 
    std::string dot_file = get_dot_file_path(img_name, dot_out_dir);
    std::string img_file = get_png_file_path(img_name, img_dir    );

    std::ofstream dot(dot_file);

    if (!dot.is_open())
        return failed_open_dot_file_message(img_name);;

    make_dump(dot);

    create_image_from_dot(dot_file, img_file);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::make_dump(std::ofstream& dot) const
{
    dot << "digraph G{\nrankdir=TB\ngraph [bgcolor=\"#000000\"];\n";

    constexpr size_t root_number = 8; // так надо
    node_dump(dot, root_.get(), root_number);

    dot << "edge[color=\"#373737\"];\n";
    create_edges(dot, root_.get());

    dot << "}" << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::node_dump(std::ofstream& dot, const node_t* node, size_t num_as_child) const
{
    msg_assert(node, "nullptr is no expect here");
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);

    std::string node_color = get_node_color(num_as_child);
    std::string node_data  = get_node_data(node, num_as_child);

    dot << "node" << node
        << "[shape=Mrecord, style=filled, fillcolor=\"" << node_color << "\", "
           "label=\"{ " << node_data << "}\", "
           "color = \"#777777\"];\n";

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get();
        if (!child_ptr) continue;
        node_dump(dot, child_ptr, child_it);
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_edges(std::ofstream& dot, const node_t* node) const
{
    msg_assert(node, "nullptr is not excpect here");

    for (size_t child_it = 0; child_it < children_quant; ++child_it)
    {
        const node_t* child_ptr = node->children[child_it].get();
        if (!child_ptr) continue;
        dot << "node" << node << "->node" << child_ptr << "\n";
        create_edges(dot, child_ptr); 
    }
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_node_color(size_t num_as_child) const
{
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);
    
    static_assert(children_quant == 8, "here is using children quant as 8");
    switch (num_as_child)
    {
        case 0: return "#1cb9ff";
        case 1: return "#00ca2c";
        case 2: return "#f31807";
        case 3: return "#FFFACD";
        case 4: return "#CD5C5C";
        case 5: return "#CDFC5C";
        case 6: return "#CFFC5C";
        case 7: return "#FF0080";
        case 8: return "#00ca2f";;
        default: builtin_unreachable_wrapper("here expect only number: 0 <= number <= 8 (8 is for root node)");
    }

    builtin_unreachable_wrapper("we must return in switch");
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_node_data(const node_t* node, size_t num_as_child) const
{
    msg_assert(node, "nullptr is no expect here");
    msg_assert(num_as_child < children_quant + 1, "num_as_child = " << num_as_child);
    
    static_assert(children_quant == 8, "here is using children quant as 8");

    std::ostringstream node_data;

    if (num_as_child == 8) node_data << "root";    
    else                   node_data << "child:\\n" << num_as_child;

    static constexpr size_t max_triangle_per_line = 7;

    const size_t node_triangles_quant = node->get_node_triangles_quant();

    if (node_triangles_quant == 0)
    {
        node_data << " | no triangles";
        goto bbox_dump;
    }

    node_data << " | triangles:";

    for (size_t triangles_it = 0, triangles_ie = node->get_node_triangles_quant(); triangles_it < triangles_ie; ++triangles_it)
    {
        if (triangles_it % max_triangle_per_line == 0 )
            node_data << "\\n";
        else
            node_data << " ";
        node_data << node->numbers_of_this_node_triangles[triangles_it];
    }

    bbox_dump:

    node_data << "| bbox:\\n"
                 "(" << node->bbox.lbp.get_x() << ", " << node->bbox.lbp.get_y() << ", " << node->bbox.lbp.get_z() << ")\\n"
                 "(" << node->bbox.rtp.get_x() << ", " << node->bbox.rtp.get_y() << ", " << node->bbox.rtp.get_z() << ")";

    return node_data.str();
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_dir(const std::string& dir) const
{
    const bool dir_already_exists =  std::filesystem::exists(dir);

    if (dir_already_exists)
        return;

    std::filesystem::create_directories(dir);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_png_file_path(std::string_view img_name, const std::string& img_dir) const
{
    std::string name(img_name);
    if (name == "")
    {
        static size_t default_name_it = 0;
        ++default_name_it;
        std::ostringstream default_name;
        default_name << "octree" << default_name_it;
        name = default_name.str();
    }

    return img_dir + name + ".png";
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
std::string
octree_t<coordinate_t>::get_dot_file_path(std::string_view dot_name, const std::string& dot_out_dir) const
{
    std::string name(dot_name);
    if (name == "")
    {
        static size_t default_name_it = 0;
        ++default_name_it;
        std::ostringstream default_name;
        default_name << "octree" << default_name_it;
        name = default_name.str();
    }

    return dot_out_dir + name + ".dot";
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::create_image_from_dot(std::string_view dot_file, std::string_view img_file) const
{
    std::ostringstream dot_command;
    dot_command << "dot -Tpng " << dot_file << " > " << img_file;
    int sys_result = std::system(dot_command.str().c_str());

    if (sys_result == EXIT_SUCCESS) return;

    std::ostringstream check_dot_command;
    check_dot_command << "which dot";
    const std::string check_dot = check_dot_command.str();

    std::cerr << RED BOLD << "Failed create\n" RESET_CONSOLE_OUT WHITE "`"
              << img_file << "'\n" RED BOLD "from\n" RESET_CONSOLE_OUT WHITE "`"
              << dot_file << "'\n\n" RED
                "Possibles reason: command " WHITE BOLD "`dot' " RESET_CONSOLE_OUT RED "didnt exists.\n"
                 RESET_CONSOLE_OUT WHITE "Check this command output: " BOLD << check_dot << RESET_CONSOLE_OUT "\n";

    sys_result = std::system(check_dot.c_str());

    if (sys_result != EXIT_SUCCESS)
    {
        std::cout << std::endl;
        return;
    }

    std::cout << WHITE "Hmm, dot was found, so, i have no idea why creating image was failed :(\n"
                 RESET_CONSOLE_OUT << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename coordinate_t>
void
octree_t<coordinate_t>::failed_open_dot_file_message(std::string_view img_name) const
{
    std::cerr << RED BOLD << "Failed open: " RESET_CONSOLE_OUT WHITE "`"
              << img_name << "'" RESET_CONSOLE_OUT << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------
