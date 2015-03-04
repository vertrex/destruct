#include "py_module.hpp"

PyMethodDef   PythonTypeBaseModule::baseTypePyMethods[2] = 
{
  {"getType", NULL, METH_CLASS, "Return the type."},
  { NULL, NULL, 0, NULL }
};

EXPORT PyTypeObject PythonBaseModule::basePyType =
{
        PyObject_HEAD_INIT(NULL)
        0,
        0,
        0,                       /* tp_basicsize */
        0,                       /* tp_itemsize */
        0,                       /* tp_dealloc */
        0,                       /* tp_print */
        0,                       /* tp_getattr */
        0,                       /* tp_setattr */
        0,                       /* tp_compare */
        0,                       /* tp_repr */
        0,                       /* tp_as_number */
        0,                       /* tp_as_sequence */
        0,                       /* tp_as_mapping */
        0,                       /* tp_hash */
        0,                       /* tp_call */
        0,                       /* tp_str */
        0,                       /* tp_getattro */
        0,                       /* tp_setattro */
        0,                       /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT |
                Py_TPFLAGS_BASETYPE,   /* tp_flags */
        0,                       /* tp_doc */
        0,                       /* tp_traverse */
        0,                       /* tp_clear */
        0,                       /* tp_richcompare */
        0,                       /* tp_weaklistoffset */
        0,                       /* tp_iter */
        0,                       /* tp_iternext */
        0,                       /* tp_method */
        0,                       /* tp_members */
        0,                       /* tp_getset */
        0,                       /* tp_base */
        0,                       /* tp_dict */
        0,                       /* tp_descr_get */
        0,                       /* tp_descr_set */
        0,                       /* tp_dictoffset */
        0,                       /* tp_init */
        0,                       /* tp_alloc */
        0,                       /* tp_new */
};

PySequenceMethods PythonBaseModule::baseSequenceMethods =
{
    0,  /* inquiry sq_length;              __len__ */
    0,  /* binaryfunc sq_concat;           __add__ */
    0,  /* intargfunc sq_repeat;           __mul__ */
    0,  /* intargfunc sq_item;             __getitem__ */
    0,  /* intintargfunc sq_slice;         __getslice__ */
    0,  /* intobjargproc sq_ass_item;      __setitem__ */
    0,  /* intintobjargproc sq_ass_slice;  __setslice__ */
};

PyMappingMethods PythonBaseModule::baseMappingMethods =
{
 0, /* mp_length*/
 0, /* mp_subscript*/
 0, /* mp_ass_subscript*/
};
