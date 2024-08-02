class Sender:
    @staticmethod
    def to_type(value, new_type):
        if new_type == "float":
            return float(value)
        elif new_type == "boolean":
            if value.lower() == "true" or value.lower() == "t":
                return True
            elif value.lower() == "false" or value.lower() == "f":
                return False
        elif new_type == "int":
            return int(value)
        else:
            return None

    def __init__(self, conn, signal_split_char):
        self.client = conn
        self.signal_split_char = signal_split_char

    def _send(self, message):
        message = message.split(self.signal_split_char)

        target_object = message[0]
        target_attribute = message[1]
        target_value = message[2]

        # vessel,throttle,1
        if target_object == "vessel":
            if target_attribute == "throttle":
                self.client.space_center.active_vessel.control.throttle = float(target_value)

        # attr = getattr(self.client.space_center.active_vessel, target_attribute)
        # attr = float(target_value)

        print(f"{target_object}, {target_attribute}, {target_value}")

    def _1send(self, message):
        message = message.split(self.signal_split_char)
        value = message[0]
        print(message)

        # current_object = None
        # for obj in message[1:]:
        #     print(obj)

    def send(self, message):
        """
        Sets an attribute based on a dot-separated path with a value on a KRPC connection object.

        :param message: A string representing the attribute path, e.g.,
        'space_center.active_vessel.control.throttle, float;1'
        """
        # Split the path into parts
        *parts, value = message.split(',')
        v_type, value = str(value).strip().split(';')

        # Traverse to the last attribute in the path
        obj = self.client
        for part in parts[:-1]:
            obj = getattr(obj, part)
        if v_type != "action":
            # Set the value on the final attribute in the path
            setattr(obj, parts[-1], self.to_type(value, v_type))
        else:
            func = getattr(obj, parts[-1])
            func()
        print(str(obj), obj)
