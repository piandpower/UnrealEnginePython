#include "UnrealEnginePythonPrivatePCH.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "Kismet2/KismetEditorUtilities.h"
#endif

#include "PythonGeneratedClass.h"

PyObject *py_unreal_engine_log(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Log, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_log_warning(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log_warning", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Warning, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_log_error(PyObject * self, PyObject * args) {
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:log_error", &py_message)) {
		return NULL;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);
	UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(message));
	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_add_on_screen_debug_message(PyObject * self, PyObject * args) {
	int key;
	float time_to_display;
	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "ifO:add_on_screen_debug_message", &key, &time_to_display, &py_message)) {
		return NULL;
	}

	if (!GEngine) {
		Py_INCREF(Py_None);
        	return Py_None;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);

	GEngine->AddOnScreenDebugMessage(key, time_to_display, FColor::Green, FString::Printf(TEXT("%s"), UTF8_TO_TCHAR(message)));

	Py_DECREF(stringified);

	Py_INCREF(Py_None);
        return Py_None;
}

PyObject *py_unreal_engine_print_string(PyObject * self, PyObject * args) {

	PyObject *py_message;
	if (!PyArg_ParseTuple(args, "O:print_string", &py_message)) {
		return NULL;
	}

	if (!GEngine) {
		Py_INCREF(Py_None);
                return Py_None;
	}

	PyObject *stringified = PyObject_Str(py_message);
	if (!stringified)
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	char *message = PyUnicode_AsUTF8(stringified);

	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Cyan, FString::Printf(TEXT("%s"), UTF8_TO_TCHAR(message)));

	Py_DECREF(stringified);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject *py_unreal_engine_get_forward_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetForwardVector(rot);
	return py_ue_new_fvector(vec);
}

PyObject *py_unreal_engine_get_right_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetRightVector(rot);
	return py_ue_new_fvector(vec);
}

PyObject *py_unreal_engine_get_up_vector(PyObject * self, PyObject * args) {
	FRotator rot;
	if (!py_ue_rotator_arg(args, rot))
		return NULL;
	FVector vec = UKismetMathLibrary::GetUpVector(rot);
	return py_ue_new_fvector(vec);
}


PyObject *py_unreal_engine_find_class(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_class", &name)) {
		return NULL;
	}

	UClass *u_class = FindObject<UClass>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_class)
		return PyErr_Format(PyExc_Exception, "unable to find class %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_class);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_find_enum(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_enum", &name)) {
		return NULL;
	}

	UEnum *u_enum = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_enum)
		return PyErr_Format(PyExc_Exception, "unable to find enum %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_enum);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}

PyObject *py_unreal_engine_load_class(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_class", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_class = StaticLoadObject(UClass::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_class)
		return PyErr_Format(PyExc_Exception, "unable to find class %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_class);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_load_enum(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_enum", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_enum = StaticLoadObject(UEnum::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_enum)
		return PyErr_Format(PyExc_Exception, "unable to find enum %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_enum);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

PyObject *py_unreal_engine_find_struct(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_struct", &name)) {
		return NULL;
	}

	UStruct *u_struct = FindObject<UStruct>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_struct)
		return PyErr_Format(PyExc_Exception, "unable to find struct %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_struct);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}

PyObject *py_unreal_engine_load_struct(PyObject * self, PyObject * args) {
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "s|s:load_struct", &name, &filename)) {
		return NULL;
	}

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_struct = StaticLoadObject(UStruct::StaticClass(), NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_struct)
		return PyErr_Format(PyExc_Exception, "unable to find struct %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_struct);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}


PyObject *py_unreal_engine_load_object(PyObject * self, PyObject * args) {
	PyObject *obj;
	char *name;
	char *filename = nullptr;
	if (!PyArg_ParseTuple(args, "Os|s:load_object", &obj, &name, &filename)) {
		return NULL;
	}

	if (!ue_is_pyuobject(obj)) {
		return PyErr_Format(PyExc_Exception, "argument is not a UObject");
	}

	ue_PyUObject *py_obj = (ue_PyUObject *)obj;
	if (!py_obj->ue_object->IsA<UClass>()) {
		return PyErr_Format(PyExc_Exception, "argument is not a UClass");
	}

	UClass *u_class = (UClass *)py_obj->ue_object;

	TCHAR *t_filename = (TCHAR *)0;
	if (filename)
		t_filename = UTF8_TO_TCHAR(filename);

	UObject *u_object = StaticLoadObject(u_class, NULL, UTF8_TO_TCHAR(name), t_filename);

	if (!u_object)
		return PyErr_Format(PyExc_Exception, "unable to load object %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}


PyObject *py_unreal_engine_find_object(PyObject * self, PyObject * args) {
	char *name;
	if (!PyArg_ParseTuple(args, "s:find_object", &name)) {
		return NULL;
	}

	UObject *u_object = FindObject<UObject>(ANY_PACKAGE, UTF8_TO_TCHAR(name));

	if (!u_object)
		return PyErr_Format(PyExc_Exception, "unable to find object %s", name);

	ue_PyUObject *ret = ue_get_python_wrapper(u_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}


PyObject *py_unreal_engine_new_object(PyObject * self, PyObject * args) {

	PyObject *obj;
	PyObject *py_outer = NULL;
	char *name = nullptr;
	if (!PyArg_ParseTuple(args, "O|Os:new_object", &obj, &py_outer, &name)) {
		return NULL;
	}

	if (!ue_is_pyuobject(obj)) {
		return PyErr_Format(PyExc_Exception, "argument is not a UObject");
	}

	ue_PyUObject *py_obj = (ue_PyUObject *)obj;

	if (!py_obj->ue_object->IsA<UClass>())
		return PyErr_Format(PyExc_Exception, "uobject is not a UClass");

	UClass *obj_class = (UClass *)py_obj->ue_object;

	FName f_name = NAME_None;

	if (name) {
		f_name = FName(UTF8_TO_TCHAR(name));
	}

	UObject *outer = GetTransientPackage();

	if (py_outer && py_outer != Py_None) {
		if (!ue_is_pyuobject(py_outer)) {
			return PyErr_Format(PyExc_Exception, "argument is not a UObject");
		}

		ue_PyUObject *py_outer_obj = (ue_PyUObject *)py_outer;

		outer = py_outer_obj->ue_object;
	}

	UObject *new_object = NewObject<UObject>(outer, obj_class, f_name, RF_Public | RF_Standalone);
	if (!new_object)
		return PyErr_Format(PyExc_Exception, "unable to create object");

	ue_PyUObject *ret = ue_get_python_wrapper(new_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}

#if WITH_EDITOR
PyObject *py_unreal_engine_create_blueprint(PyObject * self, PyObject * args) {

	PyObject *py_parent;
	char *name;
	if (!PyArg_ParseTuple(args, "Os:create_blueprint", &py_parent, &name)) {
		return NULL;
	}

	UClass *parent = UObject::StaticClass();

	if (py_parent != Py_None) {
		if (!ue_is_pyuobject(py_parent)) {
			return PyErr_Format(PyExc_Exception, "argument is not a UObject");
		}
		ue_PyUObject *py_obj = (ue_PyUObject *)py_parent;
		if (!py_obj->ue_object->IsA<UClass>())
			return PyErr_Format(PyExc_Exception, "uobject is not a UClass");
		parent = (UClass *)py_obj->ue_object;
	}

	UObject *outer = CreatePackage(nullptr, TEXT("/Game/"));
	UBlueprint *bp = FKismetEditorUtilities::CreateBlueprint(parent, outer, UTF8_TO_TCHAR(name), EBlueprintType::BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());

	ue_PyUObject *ret = ue_get_python_wrapper(bp);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;

}
#endif

PyObject *py_unreal_engine_new_class(PyObject * self, PyObject * args) {

	PyObject *py_parent;
	char *name;
	if (!PyArg_ParseTuple(args, "Os:new_class", &py_parent, &name)) {
		return NULL;
	}

	UObject *outer = GetTransientPackage();
	UClass *parent = UObject::StaticClass();

	if (py_parent != Py_None) {
		if (!ue_is_pyuobject(py_parent)) {
			return PyErr_Format(PyExc_Exception, "argument is not a UObject");
		}
		ue_PyUObject *py_obj = (ue_PyUObject *)py_parent;
		if (!py_obj->ue_object->IsA<UClass>())
			return PyErr_Format(PyExc_Exception, "uobject is not a UClass");
		parent = (UClass *)py_obj->ue_object;
	}

	UBlueprintGeneratedClass *new_object = nullptr;

	if (Cast<UBlueprintGeneratedClass>(parent)) {
		UE_LOG(LogPython, Warning, TEXT("Parent class is blueprint generated"));
		new_object = NewObject<UBlueprintGeneratedClass>(outer, UTF8_TO_TCHAR(name), RF_Public);
	}
	else {
		UE_LOG(LogPython, Warning, TEXT("Parent class is NOT blueprint generated"));
		new_object = NewObject<UBlueprintGeneratedClass>(outer, UTF8_TO_TCHAR(name), RF_Public);
		new_object->ClassFlags |= CLASS_Native;
	}

	if (!new_object)
		return PyErr_Format(PyExc_Exception, "unable to create class");

	new_object->ClassConstructor = parent->ClassConstructor;
	new_object->SetSuperStruct(parent);

	new_object->PropertyLink = parent->PropertyLink;
	new_object->ClassWithin = parent->ClassWithin;
	new_object->ClassConfigName = parent->ClassConfigName;

	new_object->ClassFlags |= (parent->ClassFlags & (CLASS_Inherit | CLASS_ScriptInherit | CLASS_HideDropDown | CLASS_Hidden));
	new_object->ClassCastFlags |= parent->ClassCastFlags;

	new_object->Bind();
	new_object->StaticLink(true);

	new_object->GetDefaultObject();

	ue_PyUObject *ret = ue_get_python_wrapper(new_object);
	if (!ret)
		return PyErr_Format(PyExc_Exception, "uobject is in invalid state");
	Py_INCREF(ret);
	return (PyObject *)ret;
}
