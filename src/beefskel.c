#include "beefskel.h"

char bskel[] = "#  The Julia version of bison.simple\n"
"#  DBM   Initial hack:        23 August  2014\n"
"#        Update to Julia 0.7   7 October 2017\n"
"#        More stuff           12 April   2018\n"
"\n"
"const YYEMPTY = -2\n"
"const YYEOF = 0\n"
"\n"
"const YYTERROR = 1\n"
"const YYERRCODE = 256\n"
"\n"
"if ! @isdefined(yydebug)\n"
"    const yydebug = false\n"
"end\n"
"\n"
"if ! @isdefined(location_tracking)\n"
"    const location_tracking = false\n"
"end\n"
"\n"
"macro yydprintf(fd, Arg)\n"
"    if yydebug\n"
"        return :(println($fd, $(esc(Arg))))\n"
"    else\n"
"        return nothing\n"
"    end\n"
"end\n"
"\n"
"# This conditionally adds all the code in blk based on\n"
"# the value of the debug flag.  The global debug flag must be\n"
"# defined before the macro defintion. \n"
"# \n"
"#  @debug_code begin\n"
"#      x = 32\n"
"#      y = 44\n"
"#      println(\"x: $x  y: $y\")\n"
"#  end\n"
"#\n"
"macro debug_code(blk)\n"
"    if yydebug\n"
"        return esc(blk)\n"
"    else\n"
"        return nothing\n"
"    end\n"
"end\n"
"\n"
"macro track_location(blk)\n"
"    if location_tracking\n"
"        return esc(blk)\n"
"    else\n"
"        return nothing\n"
"    end\n"
"end\n"
"\n"
"# macro next_token(char, lval, lloc, lex)\n"
"#     if location_tracking\n"
"#         return :(($(esc(char)), $(esc(lval)), $(esc(lloc))) = $(esc(lex))())\n"
"#     else\n"
"#         return :(($(esc(char)), $(esc(lval))) = $(esc(lex))())\n"
"#     end\n"
"# end\n"
"\n"
"\n"
"# YYINITDEPTH -- initial size of the parser's stacks.  We don't \n"
"# really change it, contrary to some of the comments still left \n"
"# below.\n"
"const YYINITDEPTH = 1000\n"
"\n"
"\n"
"# The optional parameter uopt is any user defined thing that can\n"
"# be used in the parsing actions. \n"
"function yyparse(yylex, uopt=nothing)\n"
"    #  Three stacks and their tools:\n"
"    # `yyss': related to states,\n"
"    # `yyvs': related to semantic values,\n"
"    # `yyls': related to locations.\n"
"\n"
"    #  Refer to the stacks thru separate pointers, yyoverflow will now\n"
"    #  just raise an error rather than reallocating them elsewhere.\n"
"\n"
"    #  The state stack. Needs to be integers for comparisons to work\n"
"    #  so we don't bother with macro-tizing the type\n"
"    yyss = OffsetArray(zeros(Int64, YYINITDEPTH), 0:(YYINITDEPTH-1))\n"
"    yyssp::Int = 0\n"
"\n"
"    # The semantic value stack.  The easiest thing to do is to declare\n"
"    # it in the .y spec as of type Any.\n"
"    yyvs = OffsetArray(Vector{YYSType}(undef, YYINITDEPTH), 0:(YYINITDEPTH-1))\n"
"    yyvsp::Int = 0\n"
"    local yylval::YYSType\n"
"\n"
"    # The location stack.\n"
"    @track_location begin\n"
"        yyls = OffsetArray(zeros(YYLType, YYINITDEPTH), 0:(YYINITDEPTH-1))\n"
"        yylsp = 0\n"
"    end\n"
"\n"
"    yystacksize = YYINITDEPTH\n"
"\n"
"    # The variables used to return semantic value and location from the\n"
"    # action routines.  \n"
"    yyval = \"\"   # this needs to be one of the base types of the YYSType Union\n"
"    @track_location begin\n"
"        yyloc = zero(YYLType)\n"
"        yylloc = zero(YYLType)  # source position of look ahead\n"
"    end\n"
"    \n"
"    # When reducing, the number of symbols on the RHS of the reduced\n"
"    # rule\n"
"    yylen = 0\n"
"    yyn::Int = 0\n"
"\n"
"    @yydprintf(stderr, \"Starting parse\")\n"
"\n"
"    yystate = 0\n"
"\n"
"    # Number of tokens to shift before error messages enabled.  \n"
"    yyerrstatus = 0\n"
"    yynerrs = 0\n"
"\n"
"    # YYEMPTY causes a token to be read.\n"
"    yychar::Int = YYEMPTY             \n"
"\n"
"    # Lookahead token as an internal (translated) token number.  \n"
"    yychar1::Int = 0\n"
"\n"
"    # Initialize stack pointers.\n"
"    # Waste one element of value and location stack\n"
"    # so that they stay on the same level as the state stack.\n"
"    # The wasted elements are never initialized.  \n"
"\n"
"    yyssp = 0\n"
"    yyvsp = 0\n"
"    yylsp = 0\n"
"\n"
"    # OK we could have swapped the ordering of YYSETSTATE and YYNEWSTATE\n"
"    # but I kept them the same to match bison.simple (ha!)\n"
"    @goto YYSETSTATE\n"
"\n"
"    # State Machine Starts here\n"
"    # yynewstate -- Push a new state, which is found in yystate.\n"
"    @label YYNEWSTATE\n"
"        # In all cases, when you get here, the value and location stacks\n"
"        # have just been pushed. so pushing a state here evens the stacks.\n"
"        yyssp += 1\n"
"        @goto YYSETSTATE\n"
"\n"
"    @label YYSETSTATE\n"
"        yyss[yyssp] = yystate\n"
"\n"
"        if yyssp >= yystacksize\n"
"            # Get the current used size of the three stacks, in elements.  \n"
"            yysize = yyssp\n"
"          \n"
"            yyssp = yysize\n"
"            yyvsp = yysize\n"
"            yylsp = yysize\n"
"          \n"
"            @yydprintf(stderr, \"Stack size increased to $yystacksize\")\n"
"          \n"
"            if yyssp >= yystacksize\n"
"                @goto YYABORTLAB\n"
"            end\n"
"        end\n"
"\n"
"        @yydprintf(stderr, \"Entering state $yystate\")\n"
"        @goto YYBACKUP\n"
"\n"
"\n"
"    @label YYBACKUP\n"
"        # Do appropriate processing given the current state.\n"
"        # Read a lookahead token if we need one and don't already have one.\n"
"        # First try to decide what to do without reference to lookahead token.\n"
"      \n"
"        yyn = yypact[yystate]\n"
"        if yyn == YYFLAG\n"
"            @goto YYDEFAULT\n"
"        end\n"
"      \n"
"        # Not known => get a lookahead token if don't already have one.\n"
"      \n"
"        # yychar is either YYEMPTY or YYEOF\n"
"        # or a valid token in external form.\n"
"      \n"
"        if yychar == YYEMPTY\n"
"            @yydprintf(stderr, \"Reading a token: \")\n"
"            \n"
"            yychar, yylval, yylloc = yylex()\n"
"            # this macro lets yylex only return two values if location\n"
"            # tracking is turned off\n"
"            #@next_token(yychar, yylval, yyloc, yylex)\n"
"        end\n"
"      \n"
"        # Convert token to internal form (in yychar1) for indexing tables with\n"
"        if yychar <= 0              # This means end of input.\n"
"            yychar1 = 0\n"
"            yychar = YYEOF          # Don't call YYLEX any more\n"
"            @yydprintf(stderr, \"Now at end of input.\")\n"
"        else\n"
"            yychar1 = YYTRANSLATE(yychar)\n"
"            @yydprintf(stderr, \"Next token is $yychar ($(yytname[yychar1]))\")\n"
"            @yydprintf(stderr, \"$yychar $yylval\")\n"
"        end\n"
"      \n"
"        yyn += yychar1\n"
"        if yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1\n"
"            @goto YYDEFAULT\n"
"        end\n"
"      \n"
"        yyn = yytable[yyn]\n"
"      \n"
"        # yyn is what to do for this token type in this state.\n"
"        # Negative => reduce, -yyn is rule number.\n"
"        # Positive => shift, yyn is new state.\n"
"        # New state is final state => don't bother to shift,\n"
"        # just return success.\n"
"        # 0, or most negative number => error.\n"
"      \n"
"        if yyn < 0\n"
"            if yyn == YYFLAG\n"
"                @goto YYERRLAB\n"
"            end\n"
"            yyn = -yyn\n"
"            @goto YYREDUCE\n"
"        elseif yyn == 0\n"
"            @goto YYERRLAB\n"
"        end\n"
"      \n"
"        if yyn == YYFINAL\n"
"            @goto YYACCEPTLAB\n"
"        end\n"
"      \n"
"        # Shift the lookahead token.\n"
"        @yydprintf(stderr, \"Shifting token $yychar ($(yytname[yychar1])), \")\n"
"      \n"
"        # Discard the token being shifted unless it is eof.\n"
"        if yychar != YYEOF\n"
"            yychar = YYEMPTY\n"
"        end\n"
"      \n"
"        # Now push the new values onto the stack\n"
"        yyvs[yyvsp += 1] = yylval\n"
"        @track_location begin\n"
"            yyls[yylsp += 1] = yylloc\n"
"        end\n"
"        \n"
"        # Count tokens shifted since error; after three, turn off error status.\n"
"        yyerrstatus > 0 && (yyerrstatus -= 1)\n"
"\n"
"        yystate = yyn\n"
"        @goto YYNEWSTATE\n"
"\n"
"\n"
"    # YYDEFAULT -- do the default action for the current state.\n"
"    @label YYDEFAULT\n"
"        yyn = yydefact[yystate]\n"
"        if yyn == 0\n"
"            @goto YYERRLAB\n"
"        end\n"
"        @goto YYREDUCE\n"
"\n"
"\n"
"    # YYREDUCE -- Do a reduction\n"
"    @label YYREDUCE\n"
"        # yyn is the number of a rule to reduce with.\n"
"        yylen = yyr2[yyn]\n"
"        # If YYLEN is nonzero, implement the default value of the action:\n"
"        # `$$ = $1'.\n"
"\n"
"        # FIXME! Remove all of the following?\n"
"        # Otherwise, the following line sets YYVAL to the semantic value of\n"
"        # the lookahead token.  This behavior is undocumented and Bison\n"
"        # users should not rely upon it.\n"
"        #yyval = yyvs[yyvsp - yylen + 1]\n"
"      \n"
"        # Similarly for the default location.  Let the user run additional\n"
"        # commands if for instance locations are ranges.\n"
"        @track_location begin\n"
"            yyloc = yyls[yylsp - yylen + 1]\n"
"            yyloc.last_line   = yyls[yylsp].last_line\n"
"            yyloc.last_column = yyls[yylsp].last_column\n"
"        end\n"
"      \n"
"        @debug_code begin\n"
"            println(stderr, \"Reducing via rule $yyn (line $(yyrline[yyn])), \")\n"
"            # Print the symbols being reduced, and their result.\n"
"            yyi = yyprhs[yyn]\n"
"            while yyrhs[yyi] > 0\n"
"                print(stderr, \"$(yytname[yyrhs[yyi]]) \")\n"
"                yyi += 1\n"
"            end\n"
"            println(stderr, \" -> $(yytname[yyr1[yyn]])\")\n"
"        end\n"
"      \n"
"%% actions # The action file replaces this line.\n"
"\n"
"        yyvsp -= yylen\n"
"        yyssp -= yylen\n"
"        yylsp -= yylen\n"
"            \n"
"        @debug_code begin\n"
"            yyssp1 = 0 - 1\n"
"            print(stderr, \"state stack now\")\n"
"            while yyssp1 < yyssp\n"
"                print(stderr, \" $(yyss[yyssp1 += 1])\")\n"
"            end\n"
"            print(stderr, \"\n\")\n"
"        end\n"
"      \n"
"        yyvs[yyvsp += 1] = yyval\n"
"        @track_location begin\n"
"            yyls[yylsp += 1] = yyloc\n"
"        end\n"
"      \n"
"        # Now `shift' the result of the reduction.  Determine what state\n"
"        # that goes to, based on the state we popped back to and the rule\n"
"        # number reduced by.\n"
"        yyn = yyr1[yyn]\n"
"      \n"
"        yystate = yypgoto[yyn - YYNTBASE] + yyss[yyssp]\n"
"        if yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == yyss[yyssp]\n"
"            yystate = yytable[yystate]\n"
"        else\n"
"            yystate = yydefgoto[yyn - YYNTBASE]\n"
"        end\n"
"      \n"
"        @goto YYNEWSTATE\n"
"\n"
"    # YYERRLAB -- here on detecting error\n"
"    @label YYERRLAB\n"
"        # If not already recovering from an error, report this error.  \n"
"        if yyerrstatus == 0\n"
"            yynerrs += 1\n"
"            write(stderr, \"Parse error. Recovering...\")\n"
"        end\n"
"        @goto YYERRLAB1\n"
"\n"
"\n"
"    # YYERRLAB1 -- error raised explicitly by an action\n"
"    @label YYERRLAB1\n"
"        if yyerrstatus == 3\n"
"            # If just tried and failed to reuse lookahead token after an\n"
"            # error, discard it.\n"
"        \n"
"            # return failure if at end of input\n"
"            if yychar == YYEOF\n"
"                @goto YYABORTLAB\n"
"            end\n"
"            @yydprintf(stderr, \"Discarding token $yychar ($(yytname[yychar1])).\")\n"
"            yychar = YYEMPTY\n"
"        end\n"
"         \n"
"        # Else will try to reuse lookahead token after shifting the error\n"
"        # token.\n"
"        yyerrstatus = 3             # Each real token shifted decrements this \n"
"        @goto YYERRHANDLE\n"
"          \n"
"\n"
"    # YYERRDEFAULT  Code in this state was labelled as \"wrong\" and commented\n"
"    # out in the 1.35 version of bison\n"
"    @label YYERRDEFAULT\n"
"        # empty state \n"
"        @goto YYERRPOP\n"
"\n"
"\n"
"    # YYERRPOP -- pop the current state because it cannot handle the\n"
"    #  error token\n"
"    @label YYERRPOP\n"
"        if yyssp == 1\n"
"            @goto YYABORTLAB\n"
"        end\n"
"        yyvsp -= 1\n"
"        yystate = yyss[yyssp -= 1]\n"
"        yylsp -= 1\n"
"\n"
"        @debug_code begin\n"
"            yyssp1 = -1\n"
"            print(stderr, \"Error: state stack now\")\n"
"            while yyssp1 <= yyssp\n"
"                print(stderr, \"  $(yyss[yyssp1 += 1])\")\n"
"            end\n"
"            print(stderr, \"\n\")\n"
"        end\n"
"\n"
"        @goto YYERRHANDLE\n"
"\n"
"    # YYERRHANDLE\n"
"    @label YYERRHANDLE\n"
"        yyn = yypact[yystate]\n"
"        if yyn == YYFLAG\n"
"            @goto YYERRDEFAULT\n"
"        end\n"
"        yyn += YYTERROR\n"
"        if yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR\n"
"            @goto YYERRDEFAULT\n"
"        end \n"
"        yyn = yytable[yyn]\n"
"        if yyn < 0\n"
"            if yyn == YYFLAG\n"
"                @goto YYERRPOP\n"
"            end\n"
"            yyn = -yyn\n"
"            @goto YYREDUCE\n"
"        elseif yyn == 0\n"
"            @goto YYERRPOP\n"
"        end\n"
"              \n"
"        if yyn == YYFINAL\n"
"            @goto YYACCEPTLAB\n"
"        end\n"
"               \n"
"        @yydprintf(stderr, \"Shifting error token, \")\n"
"      \n"
"        yyvs[yyvsp += 1] = yylval\n"
"        @track_location begin\n"
"            yyls[yylsp += 1] = yylloc\n"
"        end\n"
"             \n"
"        yystate = yyn\n"
"        @goto YYNEWSTATE\n"
"\n"
"\n"
"    # yyacceptlab -- YYACCEPT comes here.\n"
"    @label YYACCEPTLAB\n"
"        yyresult = 0\n"
"        @goto YYRETURN\n"
"\n"
"\n"
"    # yyabortlab -- YYABORT comes here.\n"
"    @label YYABORTLAB\n"
"        yyresult = 1\n"
"        @goto YYRETURN\n"
"\n"
"\n"
"    # YYOVERFLOWAB -- parser overflow comes here... uh but\n"
"    # nothing comes here.  I must have \"optimized\" it away.\n"
"    # Will need to look at that.\n"
"    @label YYOVERFLOWLAB\n"
"        write(stderr, \"Parser stack overflow\")\n"
"        yyresult = 2\n"
"        @goto YYRETURN\n"
"\n"
"\n"
"    @label YYRETURN\n"
"        return yyresult\n"
"\n"
"end\n"
"\n"
"\n";
