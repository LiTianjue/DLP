#ifndef MXML_DECL
#define MXML_DECL 
#endif

#  ifdef __cplusplus
extern "C" {

#  endif /* __cplusplus */

    /*
    * Prototypes...
    */

    MXML_DECL extern void		mxmlAdd(mxml_node_t *parent, int where,
        mxml_node_t *child, mxml_node_t *node);
    MXML_DECL extern void		mxmlDelete(mxml_node_t *node);
    MXML_DECL extern const char	*mxmlElementGetAttr(mxml_node_t *node, const char *name);
    extern void		mxmlElementSetAttr(mxml_node_t *node, const char *name,
        const char *value);
    MXML_DECL extern void		mxmlElementSetAttrf(mxml_node_t *node, const char *name,
        const char *format, ...)
#    ifdef __GNUC__
        __attribute__ ((__format__ (__printf__, 3, 4)))
#    endif /* __GNUC__ */
        ;
    MXML_DECL extern int		mxmlEntityAddCallback(int (*cb)(const char *name));
    MXML_DECL extern const char	*mxmlEntityGetName(int val);
    MXML_DECL extern int		mxmlEntityGetValue(const char *name);
    MXML_DECL extern void		mxmlEntityRemoveCallback(int (*cb)(const char *name));
    MXML_DECL extern mxml_node_t	*mxmlFindElement(mxml_node_t *node, mxml_node_t *top,
        const char *name, const char *attr,
        const char *value, int descend);
    MXML_DECL extern void		mxmlIndexDelete(mxml_index_t *ind);
    MXML_DECL extern mxml_node_t	*mxmlIndexEnum(mxml_index_t *ind);
    MXML_DECL extern mxml_node_t	*mxmlIndexFind(mxml_index_t *ind,
        const char *element,
        const char *value);
    MXML_DECL extern mxml_index_t	*mxmlIndexNew(mxml_node_t *node, const char *element,
        const char *attr);
    MXML_DECL extern mxml_node_t	*mxmlIndexReset(mxml_index_t *ind);
    MXML_DECL extern mxml_node_t	*mxmlLoadFd(mxml_node_t *top, int fd,
        mxml_type_t (*cb)(mxml_node_t *));
    MXML_DECL extern mxml_node_t	*mxmlLoadFile(mxml_node_t *top, FILE *fp,
        mxml_type_t (*cb)(mxml_node_t *));
    MXML_DECL extern mxml_node_t	*mxmlLoadString(mxml_node_t *top, const char *s,
        mxml_type_t (*cb)(mxml_node_t *));
    MXML_DECL extern mxml_node_t	*mxmlNewCDATA(mxml_node_t *parent, const char *string);
    MXML_DECL extern mxml_node_t	*mxmlNewCustom(mxml_node_t *parent, void *data,
        mxml_custom_destroy_cb_t destroy);
    MXML_DECL extern mxml_node_t	*mxmlNewElement(mxml_node_t *parent, const char *name);
    MXML_DECL extern mxml_node_t	*mxmlNewInteger(mxml_node_t *parent, int integer);
    MXML_DECL extern mxml_node_t	*mxmlNewOpaque(mxml_node_t *parent, const char *opaque);
    MXML_DECL extern mxml_node_t	*mxmlNewReal(mxml_node_t *parent, double real);
    MXML_DECL extern mxml_node_t	*mxmlNewText(mxml_node_t *parent, int whitespace,
        const char *string);
    MXML_DECL extern mxml_node_t	*mxmlNewTextf(mxml_node_t *parent, int whitespace,
        const char *format, ...)
#    ifdef __GNUC__
        __attribute__ ((__format__ (__printf__, 3, 4)))
#    endif /* __GNUC__ */
        ;
    MXML_DECL extern mxml_node_t	*mxmlNewXML(const char *version);
    MXML_DECL extern int		mxmlRelease(mxml_node_t *node);
    MXML_DECL extern void		mxmlRemove(mxml_node_t *node);
    MXML_DECL extern int		mxmlRetain(mxml_node_t *node);
    MXML_DECL extern char		*mxmlSaveAllocString(mxml_node_t *node,
        mxml_save_cb_t cb);
    MXML_DECL extern int		mxmlSaveFd(mxml_node_t *node, int fd,
        mxml_save_cb_t cb);
    MXML_DECL extern int		mxmlSaveFile(mxml_node_t *node, FILE *fp,
        mxml_save_cb_t cb);
    MXML_DECL extern int		mxmlSaveString(mxml_node_t *node, char *buffer,
        int bufsize, mxml_save_cb_t cb);
    MXML_DECL extern mxml_node_t	*mxmlSAXLoadFd(mxml_node_t *top, int fd,
        mxml_type_t (*cb)(mxml_node_t *),
        mxml_sax_cb_t sax, void *sax_data);
    MXML_DECL extern mxml_node_t	*mxmlSAXLoadFile(mxml_node_t *top, FILE *fp,
        mxml_type_t (*cb)(mxml_node_t *),
        mxml_sax_cb_t sax, void *sax_data);
    MXML_DECL extern mxml_node_t	*mxmlSAXLoadString(mxml_node_t *top, const char *s,
        mxml_type_t (*cb)(mxml_node_t *),
        mxml_sax_cb_t sax, void *sax_data);
    MXML_DECL extern int		mxmlSetCDATA(mxml_node_t *node, const char *data);
    MXML_DECL extern int		mxmlSetCustom(mxml_node_t *node, void *data,
        mxml_custom_destroy_cb_t destroy);
    MXML_DECL extern void		mxmlSetCustomHandlers(mxml_custom_load_cb_t load,
        mxml_custom_save_cb_t save);
    MXML_DECL extern int		mxmlSetElement(mxml_node_t *node, const char *name);
    MXML_DECL extern void		mxmlSetErrorCallback(mxml_error_cb_t cb);
    MXML_DECL extern int		mxmlSetInteger(mxml_node_t *node, int integer);
    MXML_DECL extern int		mxmlSetOpaque(mxml_node_t *node, const char *opaque);
    MXML_DECL extern int		mxmlSetReal(mxml_node_t *node, double real);
    MXML_DECL extern int		mxmlSetText(mxml_node_t *node, int whitespace,
        const char *string);
    MXML_DECL extern int		mxmlSetTextf(mxml_node_t *node, int whitespace,
        const char *format, ...)
#    ifdef __GNUC__
        __attribute__ ((__format__ (__printf__, 3, 4)))
#    endif /* __GNUC__ */
        ;
    MXML_DECL extern void		mxmlSetWrapMargin(int column);
    MXML_DECL extern mxml_node_t	*mxmlWalkNext(mxml_node_t *node, mxml_node_t *top,
        int descend);
    MXML_DECL extern mxml_node_t	*mxmlWalkPrev(mxml_node_t *node, mxml_node_t *top,
        int descend);


    /*
    * Semi-private functions...
    */

    MXML_DECL extern void		mxml_error(const char *format, ...);
    MXML_DECL extern mxml_type_t	mxml_ignore_cb(mxml_node_t *node);
    MXML_DECL extern mxml_type_t	mxml_integer_cb(mxml_node_t *node);
    MXML_DECL extern mxml_type_t	mxml_opaque_cb(mxml_node_t *node);
    MXML_DECL extern mxml_type_t	mxml_real_cb(mxml_node_t *node);


    /*
    * C++ support...
    */

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_mxml_h_ */
